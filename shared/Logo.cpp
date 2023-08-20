#include "Logo.h"

Logo::Logo() {
    logo_.emplace_back("  _____                   _             _   _____                          ");
    logo_.emplace_back(" |_   _|__ _ __ _ __ ___ (_)_ __   __ _| | |_   _|_ ___   _____ _ __ _ __  ");
    logo_.emplace_back("   | |/ _ \\ '__| '_ ` _ \\| | '_ \\ / _` | |   | |/ _` \\ \\ / / _ \\ '__| '_ \\ ");
    logo_.emplace_back("   | |  __/ |  | | | | | | | | | | (_| | |   | | (_| |\\ V /  __/ |  | | | |");
    logo_.emplace_back("   |_|\\___|_|  |_| |_| |_|_|_| |_|\\__,_|_|   |_|\\__,_| \\_/ \\___|_|  |_| |_|");
}

const std::string &Logo::getRow(int r) const {
    return logo_[r];
}

int Logo::rows() const {
    return (int)logo_.size();
}

int Logo::cols() const {
    return (int)logo_[0].size();
}