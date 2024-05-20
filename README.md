# Домашнее задание №3

1️⃣. В примере «светофор» изменить/реализовать сущность Connector, интегрировать с имитатором сервера
ГСУДД: сервер по запросу передаёт номер режима работы, connector должен его передать в систему
управления; в системе управления режимы должны быть предварительно описаны.
Ожидаемый результат: после запуска сервера и светофора появляется сообщение от системы управления о
работе по заданному режиму.
Примечание: взять за отправную точку код из fork-ов или пример из ветки example-networking.

2️⃣. Необязательное задание: Тесты функционала нового модуля Connector используя ATF (пары тестов достаточно).
Библиотека есть в SDK. Её репозиторий - https://github.com/freebsd/atf

3️⃣. Необязательное задание повышенной сложности:
вместо простого числа реализовать обработку режима в json формате и передачу в систему управления в виде
структуры данных. Можно использовать сторонние библиотеки для разбора строки в json формате.
Ожидаемый результат: после запуска сервера и светофора появляется сообщение от системы управления о
работе по заданному режиму, в системе управления при этом режимы жёстко не прописываются.
Примечание: для этого задания может быть полезным посмотреть код примера из SDK koslogger – как
использовать статические библиотеки (в koslogger используется библиотека spdlog)

4️⃣. Отправить все свои изменения в свой репозиторий на github
- в ветку day3-homework-network-basic для задания 1,
- если сделано и задание 2, то в ветку day3-homework-network-extended, прислать ссылку на репозиторий в
телеграм

# Решение

## Connector

Добавлена сущность `Connector`, получающая значение текущего режима от `CentralSystem` и передающая значение в `Control`, описана соответствующая политика безопасности.

[Базовый сценарий](//www.plantuml.com/plantuml/png/hLFDYXGn4BxtKnHpMi5jy5y6HXPMN4J5XV6ubtOwp0RJIPEaH_Rchg28mY88tXJq1HfrT7Prz1MgtyXAPzfC6X4FUqYQIdtrvQilrLl6PjfMXKWijuB105zZZT_m9tvgNk89D8VDCvpZ4klc2Ixm90NyI6T715ew49vY3PIQurTA_t17WoJ9AgjaLJnaEYc9dazucKaBWmifR3DfTIR61yQoOW2PWI8pjW-yI40b9PjOfLlGXA8OSgc5MAs41rWHvo-dS6SyRdEFZ8bpLrAuowVxrkpithxVGhYA4LTJkCcpgLI6pamBoIcEGTTI63CzOrwYqRCuVPqK2axzUe52Md9cOlaiiymtv3ROlB6oO0ZudWpytZnjdkEYEMfUhJaRAbT--GziKhcZeU8FX3sbuXVkeTPA8SbPlxgxcMoZohTOunUi-rVcB9OR8ZARbYCWRmC1BgbklZuFzPxSo8qT1RlFGy7ilif7j-wfd9qBfEwLQFqXBPNWamD_r2dDX52FEnHNGsX90WSzOpBLgYe1t-2217rsOzeExCADQi9L4FsEJd-HrheRuqCFfVtOTT8ShyIlJS9AlSb7cfLAs__NprLFlfkd8NYQmD9EcTiSX4Pg20-sT_VacDcD8lQBak65VEZhEW6QcL99mmWLqmT9VL3eXCaSMYSJykpVd5n5ZnGmFSuG-WaAVuF_ETmMTxtpzwvf-TkeO0WEq3VY3Ab2w5LkaJDL8Nu3)

![Базовый сценарий](doc/flow.png)

