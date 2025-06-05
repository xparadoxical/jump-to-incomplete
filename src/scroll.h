#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace jti::scroll
{
    void instantJumpForward(BoomScrollLayer* scrollLayer, int pages);
    void jumpForward(BoomScrollLayer *scrollLayer, int pages);
    void toIncompletePage(BoomScrollLayer *scrollLayer,
                          int pageSize, int totalItems, std::function<bool(int pageIndex, int itemIndex)> isCompleted,
                          std::function<void(BoomScrollLayer *, int pagesForward)> jumpFunction, CCNode *clickedButton);
}