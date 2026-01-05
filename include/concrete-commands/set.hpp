#include "../command.hpp"
#include "../receiver.hpp"

class SetCommand : public Command {
private:
    Receiver &receiver; 
    std::string key;
    RedisData value;

public:
    SetCommand(Receiver& rec, const std::string& k, const RedisData v)
        : receiver(rec), key(k), value(v)
    {
    }

    void execute() override;
};
