#include <vector>
#include "./tokens/AToken.hpp"
#include "./tokens/Group.hpp"
#include "./tokens/Or.hpp"
#include "./tokens/Between.hpp"
#include "./tokens/Sequence.hpp"
#include "./tokens/Single.hpp"
#include "./Pattern.hpp"

using namespace rgx;
std::vector<AToken *> get_patterns() {
    Pattern number;
    number.append(Between('0', '9'));

    Pattern ip;
    ip.append(Between('0', '9', 1 , 3))
      .append(Single(".", 1 , 1))
      .append(Between('0', '9', 1 , 3))
      .append(Single(".", 1 , 1))
      .append(Between('0', '9', 1 , 3))
      .append(Single(".", 1 , 1))
      .append(Between('0', '9', 1 , 3))

}