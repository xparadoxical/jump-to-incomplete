#include "utils.h"
#include "scroll.h"

void jumpForwardCore(BoomScrollLayer* scrollLayer, int pages, std::function<void(BoomScrollLayer*,int)> moveFn)
{
    //BoomScrollLayer::repositionPagesLooped doesn't support >1-page jumps
    log::debug("jumping {} pages forward", pages);
    for (int k = 0; k < pages; k++)
        moveFn(scrollLayer, scrollLayer->m_page + 1);
}

void jti::scroll::instantJumpForward(BoomScrollLayer* scrollLayer, int pages)
{
    jumpForwardCore(scrollLayer, pages, &BoomScrollLayer::instantMoveToPage);
}

void jti::scroll::jumpForward(BoomScrollLayer* scrollLayer, int pages)
{
    jumpForwardCore(scrollLayer, pages, &BoomScrollLayer::moveToPage);
}

/// @param scrollLayer The looped layer to scroll.
/// @param pageSize Max size of a page.
/// @param totalItems Total items in the entire scroll layer.
/// @param isCompleted Function checking if an item is unlocked. Item index is not page-relative.
/// @param jumpFunction Function invoked when an incomplete item is found.
/// @param clickedButton Button that will be hidden if nothing is found.
void jti::scroll::toIncompletePage(BoomScrollLayer* scrollLayer,
    int pageSize,
    int totalItems,
    std::function<bool(int pageIndex, int itemIndex)> isCompleted,
    std::function<void(BoomScrollLayer*, int pagesForward)> jumpFunction,
    CCNode* clickedButton)
{
    auto pageCount = scrollLayer->getTotalPages();
    auto startingPage = jti::utils::floorMod(scrollLayer->m_page, pageCount);
    log::debug("pageCount {}", pageCount);

    for (int i = 1; i <= pageCount; i++) //check the next pageCount pages
    {
        auto pageIndex = (startingPage + i) % pageCount;
        auto pageItemCount = std::min(pageSize, totalItems - pageIndex * pageSize);
        log::debug("checking page {} with {} items", pageIndex, pageItemCount);

        for (int j = 0; j < pageItemCount; j++) //check all items on the page
        {
            auto itemIndex = pageIndex * pageSize + j;

            if (!isCompleted(pageIndex, itemIndex))
            {
                jumpFunction(scrollLayer, i);
                return;
            }
        }
    }

    log::debug("no item found");
    clickedButton->setVisible(false);
}