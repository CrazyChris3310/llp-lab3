**Вахрушев Данил P33312**

# Низкоуровневое Программирование. Лабораторная работа 3

## Installation and start


### Requirements:
- cmake (3.16)
- make
- g++

```bash
cmake .
```

- Server
    ```
  make lab3-server
  ./lab3-server <port>
  ```
- Client:
    ```
  make lab3-client
  ./lab3-client <host> <port> <database>
  ```

## Цель задания
На базе данного транспортного формата описать схему протокола обмена информацией и воспользоваться
существующей библиотекой по выбору для реализации модуля, обеспечивающего его функционирование.
Протокол должен включать представление информации о командах создания, выборки, модификации и
удаления данных в соответствии с данной формой, и результатах их выполнения.

## Задачи

1. Изучить выбранную библиотеку
2. На основе существующей библиотеки реализовать модуль, обеспечивающий взаимодействие
3. Реализовать серверную часть в виде консольного приложения
4. Реализовать клиентскую часть в виде консольного приложения

### Аспекты реализации

Структура проекта:

* `client` - клиенсткое приложение
* `server` - серверное приложение
* `parser_lib` - получено в результате выполнения ЛР 2
* `dbms` - получено в результате выполнения ЛР 1
* `*.xsd` - описанные схемы сообщений
* `common` - сгенерированные по схемам файлы, общие для клиента и сервера

Для линковки модулей друг с другом используется CMake.

## Описание работы и реализации

- Xml-библиотека  поддерживающая кодогенерацию на основе схемы - `XSD Code Synthesis` 
- Сетевое взаимодействие реализовано посредством сокетов API ОС;
- Результаты запроса отсылаются клиенту пакетами, в каждом пакете максимум 10 записей из запроса.
- Клиент отправляет серверу сообщения, определенного формата, есть 2 типа сообщений
    - `connect` - запрос на подключение к определенной базе
    - `query` - запрос на исполнение запроса
- Ответ от сервера содержит в себе:
    - статус запроса
    - сообщение от ошибке
    - флажок, последний ли это ответ от сервера
    - возможный результат запроса:
        - Заголовок
        - Тело ответа

Порадок обработки запроса следующий:
1. Клиент отправляет запрос и ожидает ответа
2. Сервер начинает исполнять запрос и отправляет клиенту результат:
    - в случае с `select`, первое сообщение - это header
    - при остальных запросах в ответе есть флажок finished
    - сообщение об ошибке, если что-то пошло не так
3. Если запрос - select, то клиент запрашивает следующий пакет записей, и так пока сервер не отправит finished
4. В конце всех запросов от сервера должен приходить ответ finished.

## Результаты


```
> FOR X IN STUDS RETURN X;
Schema table doesn't exists
> FOR X IN GROUPS RETURN X;
Schema table doesn't exists

> CREATE TABLE STUDS { "stud_name": string, "stud_group_id": int };
> CREATE TABLE GROUPS { "group_id": int, "group_name": string };

> FOR X IN STUDS RETURN X;
stud_group_id       stud_name           
> FOR X IN GROUPS RETURN X;
group_name          group_id

> INSERT { "stud_name": "Vasya", "stud_group_id": 1 } INTO STUDS;
> INSERT { "stud_name": "Misha", "stud_group_id": 1 } INTO STUDS;
> INSERT { "stud_name": "Stepa", "stud_group_id": 2 } INTO STUDS;
> INSERT { "group_id": 1, "group_name": "P314" } INTO GROUPS;
> INSERT { "group_id": 2, "group_name": "N521" } INTO GROUPS;

> FOR X IN STUDS RETURN X;
stud_group_id       stud_name           
1                   Vasya               
1                   Misha               
2                   Stepa

> FOR X IN GROUPS RETURN X;
group_name          group_id            
P314                1                   
N521                2 

> FOR X IN STUDS
        FOR Y IN GROUPS
                RETURN ALL;
stud_group_id       stud_name           group_name          group_id            
1                   Vasya               P314                1                   
1                   Vasya               N521                2                   
1                   Misha               P314                1                   
1                   Misha               N521                2                   
2                   Stepa               P314                1                   
2                   Stepa               N521                2 

> FOR X IN STUDS
        FOR Y IN GROUPS
                FILTER group_id == stud_group_id
                RETURN ALL;
stud_group_id       stud_name           group_name          group_id            
1                   Vasya               P314                1                   
1                   Misha               P314                1                   
2                   Stepa               N521                2

> FOR X IN STUDS 
        RETURN X;
stud_group_id       stud_name           
1                   Vasya               
1                   Misha               
2                   Stepa               
> FOR X IN STUDS
        FILTER stud_name == "Misha"
        UPDATE X WITH { "stud_group_id": 2 } IN STUDS;
> FOR X IN STUDS RETURN X;
stud_group_id       stud_name           
1                   Vasya               
2                   Misha               
2                   Stepa

> FOR X IN STUDS
        FILTER stud_group_id == 1
        REMOVE X IN STUDS;  
> FOR X IN STUDS RETURN X;
stud_group_id       stud_name           
2                   Misha               
2                   Stepa

> DROP TABLE GROUPS;
> FOR X IN GROUPS RETURN X;
Schema table doesn't exists
```

[XML-схема запроса от клиента приведена здесь](req_schema.xsd)

[XML-схема ответа от сервера](resp_schema.xsd)

## Вывод

В процессе выполнения лабораторной работы я познакомился с понятием xml-схемы и научился применять ее для генерации парсеров.
Также я поближе познакомился с сетевым взаимодействием

# Задание

Запрос к базе Northwind.
Выбрать все заказы, которые заказала Maria Anders.

#### SQL QUERY
```sql
SELECT * FROM orders
INNER JOIN customers ON orders.customer_id = customers.customer_id
WHERE customers.contact_name = 'Maria Anders';
```

| order_id | customer_id | employee_id | order_date               | required_date            | shipped_date             | ship_via | freight | ship_name            | ship_address  | ship_city | ship_region | ship_postal_code | ship_country | customer_id | company_name        | contact_name | contact_title        | address       | city   | region | postal_code | country | phone       | fax         |
| -------- | ----------- | ----------- | ------------------------ | ------------------------ | ------------------------ | -------- | ------- | -------------------- | ------------- | --------- | ----------- | ---------------- | ------------ | ----------- | ------------------- | ------------ | -------------------- | ------------- | ------ | ------ | ----------- | ------- | ----------- | ----------- |
| 10643    | ALFKI       | 6           | 1997-08-25T00:00:00.000Z | 1997-09-22T00:00:00.000Z | 1997-09-02T00:00:00.000Z | 1        | 29.46   | Alfreds Futterkiste  | Obere Str. 57 | Berlin    |             | 12209            | Germany      | ALFKI       | Alfreds Futterkiste | Maria Anders | Sales Representative | Obere Str. 57 | Berlin |        | 12209       | Germany | 030-0074321 | 030-0076545 |
| 10692    | ALFKI       | 4           | 1997-10-03T00:00:00.000Z | 1997-10-31T00:00:00.000Z | 1997-10-13T00:00:00.000Z | 2        | 61.02   | Alfred's Futterkiste | Obere Str. 57 | Berlin    |             | 12209            | Germany      | ALFKI       | Alfreds Futterkiste | Maria Anders | Sales Representative | Obere Str. 57 | Berlin |        | 12209       | Germany | 030-0074321 | 030-0076545 |
| 10702    | ALFKI       | 4           | 1997-10-13T00:00:00.000Z | 1997-11-24T00:00:00.000Z | 1997-10-21T00:00:00.000Z | 1        | 23.94   | Alfred's Futterkiste | Obere Str. 57 | Berlin    |             | 12209            | Germany      | ALFKI       | Alfreds Futterkiste | Maria Anders | Sales Representative | Obere Str. 57 | Berlin |        | 12209       | Germany | 030-0074321 | 030-0076545 |
| 10835    | ALFKI       | 1           | 1998-01-15T00:00:00.000Z | 1998-02-12T00:00:00.000Z | 1998-01-21T00:00:00.000Z | 3        | 69.53   | Alfred's Futterkiste | Obere Str. 57 | Berlin    |             | 12209            | Germany      | ALFKI       | Alfreds Futterkiste | Maria Anders | Sales Representative | Obere Str. 57 | Berlin |        | 12209       | Germany | 030-0074321 | 030-0076545 |
| 10952    | ALFKI       | 1           | 1998-03-16T00:00:00.000Z | 1998-04-27T00:00:00.000Z | 1998-03-24T00:00:00.000Z | 1        | 40.42   | Alfred's Futterkiste | Obere Str. 57 | Berlin    |             | 12209            | Germany      | ALFKI       | Alfreds Futterkiste | Maria Anders | Sales Representative | Obere Str. 57 | Berlin |        | 12209       | Germany | 030-0074321 | 030-0076545 |
| 11011    | ALFKI       | 3           | 1998-04-09T00:00:00.000Z | 1998-05-07T00:00:00.000Z | 1998-04-13T00:00:00.000Z | 1        | 1.21    | Alfred's Futterkiste | Obere Str. 57 | Berlin    |             | 12209            | Germany      | ALFKI       | Alfreds Futterkiste | Maria Anders | Sales Representative | Obere Str. 57 | Berlin |        | 12209       | Germany | 030-0074321 | 030-0076545 |

---

#### My AQL Query
```sql
FOR X IN Customers
        FOR Y IN Orders
                FILTER CustomerID == OrderCustomerID && ContactName == "Maria Anders"
                RETURN ALL;


```

|Fax              |        Phone            |        Country        |          PostalCode      |         Region        |           City          |           Address            |      ContactTitle          |   ContactName     |         CompanyName          |    CustomerID       |        ShipCountry    |          ShipPostalCode    |       ShipRegion    |           ShipCity      |           ShipAddress       |       ShipName               |  Freight          |        ShipVia     |             ShippedDate        |      RequiredDate     |        OrderDate       |         EmployeeID    |           OrderCustomerID   |       OrderID  |
|-----------------|-------------------------|-----------------------|--------------------------|-----------------------|-------------------------|------------------------------|----------------------------|-------------------|------------------------------|---------------------|-----------------------|----------------------------|---------------------|-------------------------|-----------------------------|------------------------------|-------------------|--------------------|--------------------------------|-----------------------|------------------------|-----------------------|-----------------------------|----------------|
|030-0076545      |        030-0074321      |        Germany        |          12209           |         NULL          |           Berlin        |           Obere Str. 57      |      Sales Representative  |   Maria Anders    |         Alfreds Futterkiste  |    ALFKI            |        Germany        |          12209             |       NULL          |           Berlin        |           Obere Str. 57     |       Alfreds Futterkiste    |  29.459999        |        1           |             9/2/1997           |      9/22/1997        |        8/25/1997       |         6             |           ALFKI             |       10643    |                
|030-0076545      |        030-0074321      |        Germany        |          12209           |         NULL          |           Berlin        |           Obere Str. 57      |      Sales Representative  |   Maria Anders    |         Alfreds Futterkiste  |    ALFKI            |        Germany        |          12209             |       NULL          |           Berlin        |           Obere Str. 57     |       Alfred''s Futterkiste  |  61.020000        |        2           |             10/13/1997         |      10/31/1997       |        10/3/1997       |         4             |           ALFKI             |       10692    |                
|030-0076545      |        030-0074321      |        Germany        |          12209           |         NULL          |           Berlin        |           Obere Str. 57      |      Sales Representative  |   Maria Anders    |         Alfreds Futterkiste  |    ALFKI            |        Germany        |          12209             |       NULL          |           Berlin        |           Obere Str. 57     |       Alfred''s Futterkiste  |  23.940001        |        1           |             10/21/1997         |      11/24/1997       |        10/13/1997      |         4             |           ALFKI             |       10702    |                
|030-0076545      |        030-0074321      |        Germany        |          12209           |         NULL          |           Berlin        |           Obere Str. 57      |      Sales Representative  |   Maria Anders    |         Alfreds Futterkiste  |    ALFKI            |        Germany        |          12209             |       NULL          |           Berlin        |           Obere Str. 57     |       Alfred''s Futterkiste  |  69.529999        |        3           |             1/21/1998          |      2/12/1998        |        1/15/1998       |         1             |           ALFKI             |       10835    |                
|030-0076545      |        030-0074321      |        Germany        |          12209           |         NULL          |           Berlin        |           Obere Str. 57      |      Sales Representative  |   Maria Anders    |         Alfreds Futterkiste  |    ALFKI            |        Germany        |          12209             |       NULL          |           Berlin        |           Obere Str. 57     |       Alfred''s Futterkiste  |  40.419998        |        1           |             3/24/1998          |      4/27/1998        |        3/16/1998       |         1             |           ALFKI             |       10952    |                
|030-0076545      |        030-0074321      |        Germany        |          12209           |         NULL          |           Berlin        |           Obere Str. 57      |      Sales Representative  |   Maria Anders    |         Alfreds Futterkiste  |    ALFKI            |        Germany        |          12209             |       NULL          |           Berlin        |           Obere Str. 57     |       Alfred''s Futterkiste  |  1.210000         |        1           |             4/13/1998          |      5/7/1998         |        4/9/1998        |         3             |           ALFKI             |       11011    |

Выводы совпадают в точности до порядка столбцов.