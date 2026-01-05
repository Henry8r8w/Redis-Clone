#include "../command.hpp"
#include "../receiver.hpp"

class GetCommand : public Command {
private:
    Receiver &receiver; 
    std::string key;
    RedisData result;
   
public:
    GetCommand(Receiver& rec, const std::string& k)
        : receiver(rec), key(k) 
    {
    }

    void execute() override;
    RedisData getResult();
};
