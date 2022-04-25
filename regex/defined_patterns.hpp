#include <unordered_map>
#include "./tokens/AToken.hpp"
#include "./tokens/Group.hpp"
#include "./tokens/Or.hpp"
#include "./tokens/Between.hpp"
#include "./tokens/Sequence.hpp"
#include "./tokens/Single.hpp"
#include "./Pattern.hpp"

using namespace rgx;
unordered_map<string, Pattern> get_patterns();