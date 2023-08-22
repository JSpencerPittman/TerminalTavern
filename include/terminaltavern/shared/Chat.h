#ifndef TERMINALTAVERN_CHAT_H
#define TERMINALTAVERN_CHAT_H

#include <deque>
#include <string>

#include <ncurses.h>

#include "Player.h"
#include "Message.h"

#define ENTER 10
#define BACKSPACE 127
#define TAB 9

#define MESSAGE_LIMIT 10

class Chat {
public:
    typedef struct {
        std::string username;
        std::string contents;
    } Message;

    Chat(WINDOW* win, Player player);
    void start();

    std::string getInput() const;

    void updatePlayerID(int playerID);

    void clearBuffer();
    void popBuffer();
    void appendBuffer(char ch);
    void writeMessageHistory(const MessageHistory& messageHistory);
private:
    void writeTextInput();
    void clearTextInput();
    void clearMessageHistory();

    void drawBorder(WINDOW* win);

    WINDOW* win_;
    MessageHistory history_;
    Player player_;
    int width_, height_;

    std::string input_;
};


#endif //TERMINALTAVERN_CHAT_H
