# PCOM Homework4 - HTTP REST CLIENT

## Mantu Matei-Cristian 322CA

### Project Breakdown

This project works as a "friendly" interface for communicating with the HTTP REST server implemented by the PCOM team. It can create accounts, login and access the personal library of the account in order to add, delete or get books.

As some responses are received in the form of JSONs, I used `nlohmann::json` for an easier parsing of the objects. I really enjoyed working with this library, as I found it really easy to use and very convenient.

In order to respond to queries, the program creates and sends an HTTP Request to the server. For each query sent, a new connection is made, which is closed after the response is received(in order to not burden the server during down time). The responses are mainly parsed using `std::find` and the json library. Error messages are forwarded to the user, if sent by the server.

Furthermore, in the case of `add_book`, the client checks for input sanity and won't send requests that do not match the correct types. The JWT token is stored in a local variable inside main and is deleted after logout, in order to not give access to the next user. The program assumes that the session cookie is invalidated by the server upon logging out.

Some functions are taken from lab, while for others I have opted to translate them into C++ for a better integration within the whole project. Overall, the lab was very useful in learning how to work with HTTP requests.

### Notes / Observations

TIL that C++ strings are mutable. Seeing all the concatenations used for formatting the http request, I was wondering if there would have been a big resource penalty for doing so, similar to Java. I found [this on the web](https://stackoverflow.com/questions/28442719/are-c-strings-mutable-unlike-java-strings) which explains that strings are mutable although string literals are not.

While doing this project, I wanted to learn more about C++ so I tried to "push the boundaries" and write syntax which I could not have written in bare C(using map and std::functional instead of a switch case, writing code with no memory leaks by making use of RAII, using overridden operators such as string concatenation, etc.). I am pleased with the result and I enjoyed solving this assignment.
