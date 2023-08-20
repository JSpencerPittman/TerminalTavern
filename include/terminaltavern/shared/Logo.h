#ifndef TERMINALTAVERN_LOGO_H
#define TERMINALTAVERN_LOGO_H

#include <vector>
#include <string>

class Logo {
public:
    Logo();

    const std::string& getRow(int r) const;
    int rows() const;
    int cols() const;
private:
    std::vector<std::string> logo_;
};


#endif //TERMINALTAVERN_LOGO_H
