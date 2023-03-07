#include "../liba/evaluate.h"
#include "../liba/exceptions.h"
#include <vector>

map_t makeMap(MapNode* node) {
    map_t mapa;
    for (MapEntry* entry : node->entries) {
        constant_t constant(entry->value->getStrType(), entry->value->getStrVal());
        map_t::entry_type ent(entry->key, constant);
        mapa.entry().push_back(ent);
    }
    return mapa;
}

create_t evaluateCreate(CreateTableNode* node) {
    MapNode* fields = node->fields;
    map_t mapa = makeMap(fields);

    return create_t(node->table, mapa);
}

drop_t evaluateDrop(DropTableNode* node) {
    return drop_t(node->table);
}

void resolvePredicate(Predicate* pred, std::vector<condition_t>& conditions) {
    if (pred->nodeType == CONDITION_NODE) {
        Condition* cond = (Condition*)pred;
        if (cond->lval->type != REF) {
            throw UnsupportedSyntaxException("Lvalue must me a reference");
        }
        constant_t constant(cond->rval->getStrType(), cond->rval->getStrVal());
        condition_t c(cond->operation_str[cond->op], cond->lval->getStrVal(), constant);
        conditions.push_back(c);
    } else if (pred->nodeType == CONDITION_UNION_NODE) {
        ConditionUnion* un = (ConditionUnion*)pred;
        if (un->op != AND) {
            throw UnsupportedSyntaxException("Only AND operator is allowed in FILTER clause");
        }
        resolvePredicate(un->lval, conditions);
        resolvePredicate(un->rval, conditions);
    }
}

void evaluateSelect(ForNode* node,std::vector<const char*>& joins, 
                               std::vector<condition_t>& conditions, ret_val_t* retVal) {
    joins.push_back(node->tableName);
    for (Node* action : ((ActionNode*)(node->action))->actions) {
        if (action->nodeType == FILTER_NODE) {
            Predicate* pred = ((FilterNode*)action)->predicate;
            resolvePredicate(pred, conditions);
        }
        if (action->nodeType == FOR_NODE) {
            evaluateSelect((ForNode*)action, joins, conditions, retVal);
        }
        if (action->nodeType == RETURN_NODE) {
            ret_val_t toReturn;
            ReturnAction* act = ((ReturnAction*)action);
            if (act->retVal->nodeType == CONSTANT_NODE) {
                Constant* constant = (Constant*)act->retVal;
                constant_t cnst(constant->getStrType(), constant->getStrVal());
                toReturn.constant(cnst);
            } else if (act->retVal->nodeType == MAP_NODE) {
                map_t mapa = makeMap((MapNode*)act->retVal);
                toReturn.map(mapa);
            }
            *retVal = toReturn;
        }
    }
}


select_t evaluateSelect(ForNode* node) {
    std::vector<const char*> joins;
    std::vector<condition_t> conditions;
    ret_val_t retVal;
    evaluateSelect(node, joins, conditions, &retVal);

    predicate_t pred;
    for (condition_t cnd : conditions) {
        pred.condition().push_back(cnd);
    }

    select_t slct(pred, retVal);

    for (const char* tbl : joins) {
        slct.table().push_back(tbl);
    }

    return slct;
}

update_t evaluateUpdate(ForNode* node) {
    std::vector<condition_t> conditions;
    map_t mapa;

    for (Node* action : ((ActionNode*)(node->action))->actions) {
        if (action->nodeType == FILTER_NODE) {
            Predicate* pred = ((FilterNode*)action)->predicate;
            resolvePredicate(pred, conditions);
        }
        if (action->nodeType == FOR_NODE) {
            throw UnsupportedSyntaxException("Update with joins is not supported");
        }
        if (action->nodeType == UPDATE_NODE) {
            UpdateAction* act = ((UpdateAction*)action);
            mapa = makeMap(act->value);
        }
    }

    predicate_t pred;
    for (condition_t cnd : conditions) {
        pred.condition().push_back(cnd);
    }

    return update_t(node->tableName, mapa, pred);
}

delete_t evaluateDelete(ForNode* node) {
    std::vector<condition_t> conditions;
    map_t mapa;

    for (Node* action : ((ActionNode*)(node->action))->actions) {
        if (action->nodeType == FILTER_NODE) {
            Predicate* pred = ((FilterNode*)action)->predicate;
            resolvePredicate(pred, conditions);
        }
        if (action->nodeType == FOR_NODE) {
            throw UnsupportedSyntaxException("Update with joins is not supported");
        }
    }

    predicate_t pred;
    for (condition_t cnd : conditions) {
        pred.condition().push_back(cnd);
    }

    return delete_t(node->tableName, pred);
}

insert_t evaluateInsert(InsertNode* node) {
    MapNode* fields = node->map;
    map_t mapa = makeMap(fields);

    return insert_t(node->table, mapa);
}


request_t toXmlRequest(NodeWrapper& wrapper) {
    Node* node = wrapper.node;

    QueryType type = wrapper.queryType;

    request_t req(getStringQueryType(type));

    switch (type) {
        case CREATE_QUERY:
            req.create(evaluateCreate((CreateTableNode*)node));
            break;
        case DROP_QUERY:
            req.drop(evaluateDrop((DropTableNode*)node));
            break;
        case INSERT_QUERY:
            req.insert(evaluateInsert((InsertNode*)node));
            break;
        case SELECT_QUERY:
            req.select(evaluateSelect((ForNode*)node));
            break;
        case UPDATE_QUERY:
            req.update(evaluateUpdate((ForNode*)node));
            break;
        case DELETE_QUERY:
            req.delete_(evaluateDelete((ForNode*)node));
            break;
    }

    return req;
}
