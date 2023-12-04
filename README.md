# 简介

基于AMQP-CPP和Qt的TCP客户端实现的C++跨平台RabbitMq客户端通信类，使用QTcpSocket完成了AMQP-CPP的网络层


### 背景
---

由于项目需要，客户端需要实现连接RabbitMq服务，并启用心跳(heartbeat)功能

调研了一些C++常用的rabbitmq客户端，rabbitmq-c、SimpleAmqpClient和AMQP-CPP

- rabbitmq-c

    原生的C语言库，无面向对象特性，易用性差，有心跳功能，但需要自行封装
    
- SimpleAmqpClient

    基于rabbitmq-c的C++封装，有面向对象特性，易用性好，但目前版本没有心跳功能

- AMQP-CPP

    有面向对象特性，易用性好，最新版本有心跳功能，Linux环境下开箱即用，windows需要自行实现网络层

### 跨平台
---

底层使用了跨平台的Qt库实现Tcp客户端，所以可以跨平台移植。

编译AMQP-CPP时，无论是windows还是Linux下，使用默认即可，即 AMQP-CPP_BUILD_SHARED, CPP_LINUX_TCP 保持OFF


### 依赖
---

AMQP-CPP 的地址是：https://github.com/CopernicaMarketingSoftware/AMQP-CPP

AMQP-CPP 提供了仅支持Linux的 TCP模块，可以在Linux上直接使用。

但在Windows上需要自行实现网络层TCP通信及IO操作。


### 参考
---

本项目受[guotianqing](https://github.com/guotianqing)启发

他基于boost io实现网络层，项目地址为：https://github.com/guotianqing/RmqMgr


### 最后
---

向各位前辈们致敬。

欢迎参考，并提出意见。
