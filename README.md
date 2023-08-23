# TerminalTavern
This is a simple digital tavern that takes place within the confines of your terminal. Create your own character with a 3x3 pixel map alongside your username.

### Motivation
Before this project I was always intimated by networking therefore I felt a mini online tavern would be the perfect way to learn this concept. I can say with 
astute confidence that I've learned a great deal about the fundamentals of networking, multithreading, as well as build systems to help programs run fluently 
on multiple operating systems.

### Required Libraries
To run TerminalTavern CMAKE must be able to find the following packages using the `find_package` command:<br>
1. Boost ASIO (for networking)
2. Nlohmann JSON (for serializing data)
3. NCurses (for creating the display) - Client Only


### Supported Operating Systems
- Ubuntu Jammy (22.04)
- MacOS (Ventura)

Any Unix based system should work as long as the librarys listed are installed, 
these are just the ones that have been confirmed. Windows support will be added at a later date.

### How to run the code
First download and build the code
```bash
git clone https://github.com/JSpencerPittman/TerminalTavern.git
cd TerminalTavern
cmake .
cmake --build .
```
next if you're a client run
```bash
./Client
```
otherwise for server
```bash
./Server <port-num>
```

### Connection?
By default my Ubuntu Jammy Desktop runs a Server that is accessible globally at terminaltavern.ddns.net, however this server will rarely
  be up since its more of a fun proof of concept than a professional product with a steady player base. 

Luckily, running your own server is as easy as running the Client application itself:
`./Server <port-number>`<br>
If you don't feel particulary picky and just run `./Server` it will run on port **9999** by default.

Anyways on the client side you can either put in a typical IPv4 address such as XXX.XXX.XXX.XXX, or somesort of hostname that can be resolved by a DNS lookup.
