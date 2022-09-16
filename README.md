# verse 0.0
## дисклеймер
1) Программа не соответствует спецификациям http, а только пародирует их выполнение, работая только с самыми основными пунктами, с которыми я сталкивался в процессе работы.
2) Работоспособность не гарантируется - это велосипед на шатких костылях
## используемые сторонние библиотеки

* JSON for Modern C++  version 3.10.4  https://github.com/nlohmann/json



## описание работы сервера
Сервер состоит из 3х файлов, из 3-х объектов, унаследованных друг от друга паровозиком.
* usrv - отвечает за создание сокета, привязывание адреса и мультиплексирование событий на сокете с помощью механизма select
* uhttp - разбивает данные на сообщения, делит их на header, body согласно протоколу http
* uhtml - анализирует заголовки/тело/мета-инфу и TODO: будет мультиплесировать вызов нужных функций

общение сервера с клиентом будет реализовано похожим на REST (таким, каким я его вижу, каким я его себе представляю, прочитав о нём только статью на википедии)

## описание файла schema.json
#### текущее состояние
```json
"index": //<-- запрос от клиента WWW.MMM.RU/index
    {
        "path":"content/test.html", //<-- этот файл мапим на запрос
        "code":200,                 //<-- статус ответа 200 ОК
        "headers":                  //<-- HTTP заголовки
        {
            "content-type":"text/html"
        }
	
    },
```
#### TODO
```json
"index": //<-- запрос от клиента WWW.MMM.RU/index
    {
        "path":"content/test.html", //<-- этот файл мапим на запрос
        "function":"indexService"   //<-- вызвать функцию indexService*
        "code":200,                 //<-- статус ответа 200 ОК
        "headers":                  //<-- HTTP заголовки
        {
            "content-type":"text/html"
        }
	
    },
```
\* функция будет храниться в unordered_map структуре:
```c++
// функция сервиса принимает POST данные запроса
typedef void(*_serviceFunction)(u_socket conn, json POST_data_parsed_to_JSON)
// структура
std::unordered_map<
    std::string, 
    _serviceFunction
> services;
//----------------------------------------------------------------
// регистрация функций
    services.insert(std::pair<...>("indexService", anyServiceFunction));
    ...

```

## Поддерживаемые платформы

    __linux__       Defined on Linux                            [x]
    __sun           Defined on Solaris                          [ ]
    __FreeBSD__     Defined on FreeBSD                          [ ]
    __NetBSD__      Defined on NetBSD                           [ ]
    __OpenBSD__     Defined on OpenBSD                          [ ]
    __APPLE__       Defined on Mac OS X                         [ ]
    __hpux          Defined on HP-UX                            [ ]
    __osf__         Defined on Tru64 UNIX (formerly DEC OSF1)   [ ]
    __sgi           Defined on Irix                             [ ]
    _AIX            Defined on AIX                              [ ]
    _WIN32          Defined on Windows                          [x]

# verse 0.1
//TODO

Have fun !

