#ifndef __ZZ_COMMON_H_
#define __ZZ_COMMON_H_

#include <cdroid.h>

/*******************************************************************
 * cdroid 源码需修改
 *      将recyclerview.cc:3760  mRV->scrollStep(unconsumedX,unconsumedY, mRV->mScrollStepConsumed);
 *                       更改为  mRV->scrollStep(unconsumedX,unconsumedY, mRV->mReusableIntPair);
 */

/*******************************************************************
 *名称：             setRecyclerViewScrollLine
 *功能：             设置自定义 RecyclerView 的滚动条
 *入口参数：          mRvHx          当前 recyclerview 指针
 *                   viewLine       滚动条滑块
 *                   orientation    滑动方向
 *
 * xml:         <FrameLayout
                    android:layout_width="wrap_content"
                    android:layout_height="wrap_content"
                    android:layout_alignParentEnd="true"
                    android:layout_marginEnd="50dp"
                    android:layout_centerVertical="true">

                    <View
                        android:layout_width="6dp"
                        android:layout_height="250dp"
                        android:background="#FFFFFF" />

                    <View
                        android:id="@+id/main_line"
                        android:layout_width="5dp"
                        android:layout_height="50dp"
                        android:background="#E91E63" />

                </FrameLayout>
 *******************************************************************/
void setRecyclerViewScrollLine(RecyclerView *mRvHx, ViewGroup *viewLine, int orientation);

void setScollViewLine(ScrollView *scrollview, ViewGroup *viewLine, int orientation);

void setViewPagerLine(ViewPager *viewPager, ViewGroup *viewLine, int orientation);

// void setNumberPickerLine(NumberPicker *numberPicker, ViewGroup *viewLine, int orientation, int wheelItemCount, int &numberPickerValue, float &nowPosition);

#endif