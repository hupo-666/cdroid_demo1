#include "zzCommon.h"
#include "paint.h"

void setRecyclerViewScrollLine(RecyclerView *mRvHx, ViewGroup *viewLine, int orientation)
{
    RecyclerView::OnScrollListener scrollListener;
    if (orientation == LinearLayout::HORIZONTAL)
    {
        scrollListener.onScrolled = [viewLine](RecyclerView &recyclerView, int dx, int dy)
        {
            int offset = recyclerView.computeHorizontalScrollOffset();
            int range = recyclerView.computeHorizontalScrollRange();
            int extent = recyclerView.computeHorizontalScrollExtent();

            float percentage = (float)offset / (float)(range - extent);
            int parentWidth = (viewLine->getParent())->getWidth();
            int mLineWidth = viewLine->getWidth();
            int position = (int)(percentage * (parentWidth - mLineWidth));
            viewLine->setTranslationX(position);
        };
    }
    else if (orientation == LinearLayout::VERTICAL)
    {
        scrollListener.onScrolled = [viewLine](RecyclerView &recyclerView, int dx, int dy)
        {
            int offset = recyclerView.computeVerticalScrollOffset();
            int range = recyclerView.computeVerticalScrollRange();
            int extent = recyclerView.computeVerticalScrollExtent();

            float percentage = (float)offset / (float)(range - extent);
            int parentHeight = (viewLine->getParent())->getHeight();
            int mLineHeight = viewLine->getHeight();
            int position = (int)(percentage * (parentHeight - mLineHeight));
            viewLine->setTranslationY(position);
        };
    }
    mRvHx->addOnScrollListener(scrollListener);
}

void setScollViewLine(ScrollView *scrollview, ViewGroup *viewLine, int orientation)
{
    switch (orientation)
    {
    case LinearLayout::HORIZONTAL:
    {
        scrollview->setOnScrollChangeListener([scrollview, viewLine](View &v, int l, int t, int oldl, int oldt)
        {
            int maxWidth = (scrollview->getChildAt(0))->getWidth();
            float percent = (float) l / (float) (maxWidth - scrollview->getWidth());
            int parentWidth = (viewLine->getParent())->getWidth();
            int scrollbarWidth = viewLine->getWidth();
            int position = (int) (percent * (parentWidth - scrollbarWidth));
            viewLine->setTranslationX(position);
        });
        break;
    }
    case LinearLayout::VERTICAL:
    {
        scrollview->setOnScrollChangeListener([scrollview, viewLine](View &v, int l, int t, int oldl, int oldt)
            {
            int maxHeight = (scrollview->getChildAt(0))->getHeight();
            float percent = (float) t / (float) (maxHeight - scrollview->getHeight());
            int parentHeight = (viewLine->getParent())->getHeight();
            int scrollbarHeight = viewLine->getHeight();
            int position = (int) (percent * (parentHeight - scrollbarHeight));
            viewLine->setTranslationY(position);
        });
        break;
    }
    default:
        break;
    }
}

void setViewPagerLine(ViewPager *viewPager, ViewGroup *viewLine, int orientation)
{
    switch (orientation)
    {
    case LinearLayout::HORIZONTAL:
    {
        ViewPager::OnPageChangeListener pageChangeListener;
        pageChangeListener.onPageScrolled = [viewPager, viewLine](int position, float positionOffset, int positionOffsetPixels)
        {
            float percent = (float)(positionOffset + position) / (float)(viewPager->getChildCount() - 1);
            int parentWidth = (viewLine->getParent())->getWidth();
            int pageHomeLineWidth = viewLine->getWidth();
            int pos = (int)(percent * (parentWidth - pageHomeLineWidth));
            viewLine->setTranslationX(pos);
        };
        viewPager->addOnPageChangeListener(pageChangeListener);
        break;
    }
    case LinearLayout::VERTICAL:
    {
        ViewPager::OnPageChangeListener pageChangeListener;
        pageChangeListener.onPageScrolled = [viewPager, viewLine](int position, float positionOffset, int positionOffsetPixels)
        {
            float percent = (float)(positionOffset + position) / (float)(viewPager->getChildCount() - 1);
            int parentHeight = (viewLine->getParent())->getHeight();
            int pageHomeLineHeight = viewLine->getHeight();
            int pos = (int)(percent * (parentHeight - pageHomeLineHeight));
            viewLine->setTranslationY(pos);
        };
        viewPager->addOnPageChangeListener(pageChangeListener);
        break;
    }
    default:
        break;
    }
}

/**
 * 暂时不用，先使用 RVNumberPicker
 * 
 * TODO: float 有点误差，会累积，后面修改
 */
// void setNumberPickerLine(NumberPicker *numberPicker, ViewGroup *viewLine, int orientation, int wheelItemCount, int &numberPickerValue, float &nowPosition)
// {LOGE("wheelItemCount = %d", wheelItemCount);
//     switch (orientation)
//     {
//     case LinearLayout::HORIZONTAL:
//     {
//         numberPicker->setOnScrollChangeListener([numberPicker, viewLine, wheelItemCount, &numberPickerValue, &nowPosition](View &v, int l, int t, int oldl, int oldt)
//         {
//             float maxWidth = numberPicker->getWidth() - numberPicker->getWidth() / wheelItemCount;
//             float percent;
//             int numberPickerLineMaxWidth = (viewLine->getParent())->getWidth() - viewLine->getWidth();
//             int nowValue = numberPicker->getValue();
//             // LOGV("nowPosition=%f",nowPosition);
//             // LOGE("numberPickerValue=%d, nowValue = %d", numberPickerValue, nowValue);

//             if (nowValue == numberPickerValue)
//             {
//                 percent = (float) std::abs(oldl - l) / maxWidth;
//                 // LOGV("percent = %f",percent);
//                 nowPosition = l < oldl ? nowPosition + percent * (float) numberPickerLineMaxWidth : nowPosition - percent * (float) numberPickerLineMaxWidth;
//                 viewLine->setTranslationX(nowPosition);
//             } else if (nowValue != numberPickerValue)
//             {
//                 if (oldl != 0)
//                 {
//                     percent = nowValue > numberPickerValue ? ((float) numberPicker->getWidth() / ((float) wheelItemCount) - (float) l + (float) oldl) / maxWidth : ((float) numberPicker->getWidth() / ((float) wheelItemCount) + (float) l - (float) oldl) / maxWidth;
//                 } else {
//                     percent = nowValue > numberPickerValue ? ((float) numberPicker->getWidth() / ((float) wheelItemCount) - (float) (l + 1)) / maxWidth : (float) (l + 1) / maxWidth;
//                 }
//                 // LOGV("percent = %f",percent);
//                 nowPosition = nowValue > numberPickerValue ? nowPosition + percent * numberPickerLineMaxWidth : nowPosition - percent * numberPickerLineMaxWidth;
//                 viewLine->setTranslationX(nowPosition);
//                 numberPickerValue = nowValue;
//             }
//             viewLine->getParent()->invalidate();
//             // LOGV("l=%d,t=%d,oldl=%d,oldt=%d,maxWidth=%f,nowPosition=%f",l,t,oldl,oldt,maxWidth,nowPosition);
//         });
//         break;
//     }
//     case LinearLayout::VERTICAL:
//     {
        
//         break;
//     }
//     default:
//         break;
//     }
// }