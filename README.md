# gRPC_CPP_CMake

- Motivation is taken from [here](https://github.com/faaxm/exmpl-cmake-grpc)
- Source of [Error-Handling](https://avi.im/grpc-errors/), [Source Code](https://github.com/avinassh/grpc-errors)

- Info related to SSL_TSL [1](https://github.com/grpc/grpc/issues/9593), [1.1](https://stackoverflow.com/questions/48024793/error-in-grpc-with-tls-support), [2](https://stackoverflow.com/questions/40623793/use-ssl-in-grpc-client-server-communication/41051292#41051292)
- [Way to build gRPC](https://stackoverflow.com/questions/63523196/how-to-build-grpc-with-openssl-instead-of-boringssl) to enable SSL/TSL. Though not sure about the result
- [This](https://github.com/grpc/grpc/issues/24918#issuecomment-741103013) solved the issue of ssl/tsl auth. This has taken `key .. cert` from gRPC source

- A [source in C#](https://blog.noser.com/grpc-tutorial-teil-2-streaming-mit-grpc/) for stream message
- A finding [regarding gRPC stream](https://stackoverflow.com/questions/67784384/c-grpc-clientasyncreaderwriter-how-to-check-if-data-is-available-for-read)
- [Message length setting in gRPC](https://nanxiao.me/en/message-length-setting-in-grpc/)
- Max size of message [rcv or sending?](https://github.com/grpc/grpc/issues/7882#issuecomment-242831397)