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
                               std::vector<condition_t>& conditions) {
    joins.push_back(node->tableName);
    for (Node* action : ((ActionNode*)(node->action))->actions) {
        if (action->nodeType == FILTER_NODE) {
            Predicate* pred = ((FilterNode*)action)->predicate;
            resolvePredicate(pred, conditions);
        }
        if (action->nodeType == FOR_NODE) {
            evaluateSelect((ForNode*)action, joins, conditions);
        }
        if (action->nodeType == RETURN_NODE) {
            ReturnAction* act = ((ReturnAction*)action);
            if (act->retVal->nodeType == CONSTANT_NODE) {
                Constant* constant = (Constant*)act->retVal;
                if (constant->type != REF) {
                    throw UnsupportedSyntaxException("Only single loop references and ALL are supported in return statement");
                }
            } else if (act->retVal->nodeType != RET_ALL_NODE) {
                throw UnsupportedSyntaxException("Only single loop references and ALL are supported in return statement");
            }
        }
    }
}


select_t evaluateSelect(ForNode* node) {
    std::vector<const char*> joins;
    std::vector<condition_t> conditions;
    evaluateSelect(node, joins, conditions);

    select_t slct;

    if (!conditions.empty()) {
        predicate_t pred;
        for (condition_t cnd : conditions) {
            pred.condition().push_back(cnd);
        }
        slct.predicate(pred);
    }

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

    update_t upd(node->tableName, mapa);

    if (!conditions.empty()) {
        predicate_t pred;
        for (condition_t cnd : conditions) {
            pred.condition().push_back(cnd);
        }
        upd.predicate(pred);
    }

    return upd;
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

    delete_t del(node->tableName);

    if (!conditions.empty()) {
        predicate_t pred;
        for (condition_t cnd : conditions) {
            pred.condition().push_back(cnd);
        }
        del.predicate(pred);
    }

    return del;
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
