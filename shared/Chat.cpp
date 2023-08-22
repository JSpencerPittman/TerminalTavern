#include "Chat.h"

#include <utility>

Chat::Chat(WINDOW *win, Player player)
    : win_(win), player_(std::move(player)),
      width_(getmaxx(win)), height_(getmaxy(win)), history_(MESSAGE_LIMIT) {
}

void Chat::start() {
    // Setup main window
    clear();
    drawBorder(win_);
    refresh();
    nodelay(win_, TRUE);

    // Create the top label
    std::string label = "Terminal Tavern";
    WINDOW* labelWindow = newwin(3, width_, 0, 0);
    drawBorder(labelWindow);
    mvwprintw(labelWindow, 1, (width_-(int)label.size())/2, label.c_str());
    wrefresh(labelWindow);

    // Create input prompt
    mvprintw(height_-2, 1, (player_.getUsername() + '>').c_str());
    refresh();
}

std::string Chat::getInput() const {
    return input_;
}

void Chat::updatePlayerID(int playerID) {
    player_.updateID(playerID);
}

void Chat::clearBuffer() {
    // Clear the texts
    clearTextInput();
    input_.clear();
//    clearMessageHistory();

    // Update Message History
//    Message newMessage = {player_.getUsername(), input_};
//    messageHistory_.push_back(newMessage);
//    if(messageHistory_.size() > MESSAGE_LIMIT) messageHistory_.pop_front();
//    input_.clear();

    // Update screen
//    writeMessageHistory();
}

void Chat::popBuffer() {
    clearTextInput();
    input_.pop_back();
    writeTextInput();
}

void Chat::appendBuffer(char ch) {
    input_.push_back(ch);
    writeTextInput();
}

void Chat::writeTextInput() {
    mvprintw(height_-2, (int)player_.getUsername().size() + 2, input_.c_str());
}

void Chat::clearTextInput() {
    std::string filler(input_.size(), ' ');
    mvprintw(height_-2, (int)player_.getUsername().size() + 2, filler.c_str());
}

void Chat::writeMessageHistory(const MessageHistory& messageHistory) {
    clearMessageHistory();
    history_ = messageHistory;
    for(int i = 0; i < (int)messageHistory.size(); i++) {
        std::string message = messageHistory.at(i).username() + '>'
                + messageHistory.at(i).content();
        mvprintw(i+3, 1, message.c_str());
    }
}

void Chat::clearMessageHistory() {
    for(int i = 0; i < (int)history_.size(); i++) {
        int len = (int)history_.at(i).username().size() + 2
                + (int)history_.at(i).content().size();
        std::string filler(len, ' ');
        mvprintw(i+3, 1, filler.c_str());
    }
}


void Chat::drawBorder(WINDOW* win) {
    attron(A_BOLD);
    box(win, 0, 0);
    attroff(A_BOLD);
}