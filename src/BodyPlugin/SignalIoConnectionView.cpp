#include "SignalIoConnectionView.h"
#include "SignalIoConnectionMapItem.h"
#include <cnoid/ViewManager>
#include <cnoid/TargetItemPicker>
#include <cnoid/Buttons>
#include <QBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QDoubleSpinBox>
#include "gettext.h"

using namespace std;
using namespace cnoid;

namespace cnoid {

class SignalIoConnectionView::Impl : public QTableWidget
{
public:
    SignalIoConnectionView* self;
    TargetItemPicker<SignalIoConnectionMapItem> targetItemPicker;
    QLabel targetLabel;

    Impl(SignalIoConnectionView* self);
    void setConnectionMapItem(SignalIoConnectionMapItem* item);
    void onAddButtonClicked();
    void onRemoveButtonClicked();
};

}


void SignalIoConnectionView::initializeClass(ExtensionManager* ext)
{
    ext->viewManager().registerClass<SignalIoConnectionView>(
        "SignalIoConnectionView", N_("I/O Connection"), ViewManager::SINGLE_OPTIONAL);
}


SignalIoConnectionView::SignalIoConnectionView()
{
    impl = new Impl(this);
}


SignalIoConnectionView::Impl::Impl(SignalIoConnectionView* self)
    : self(self),
      targetItemPicker(self)
{
    self->setDefaultLayoutArea(View::RIGHT);

    auto vbox = new QVBoxLayout;
    vbox->setSpacing(0);

    int hs = self->style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);
    
    auto hbox = new QHBoxLayout;
    hbox->setSpacing(0);
    hbox->addSpacing(hs);
    targetLabel.setStyleSheet("font-weight: bold");
    hbox->addWidget(&targetLabel, 0, Qt::AlignVCenter);
    hbox->addSpacing(hs);
    auto addButton = new PushButton(_("Add"));
    addButton->sigClicked().connect([&](){ onAddButtonClicked(); });
    hbox->addWidget(addButton);
    auto removeButton = new PushButton(_("Remove"));
    removeButton->sigClicked().connect([&](){ onRemoveButtonClicked(); });
    hbox->addWidget(removeButton);
    hbox->addStretch();
    vbox->addLayout(hbox);

    // Setup the table
    auto hframe = new QFrame;
    hframe->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    vbox->addWidget(hframe);
    setFrameShape(QFrame::NoFrame);
    setColumnCount(6);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setTabKeyNavigation(true);

    setHorizontalHeaderLabels({_("Output Device"), _("Signal No."), " ", _("Input Device"), _("Signal No."), ""});

    auto hheader = horizontalHeader();
    hheader->setSectionResizeMode(QHeaderView::ResizeToContents);
    hheader->setStretchLastSection(true);

    auto vheader = verticalHeader();
    vheader->hide();
    vheader->setSectionResizeMode(QHeaderView::ResizeToContents);

    vbox->addWidget(this);
    self->setLayout(vbox);

    targetItemPicker.sigTargetItemChanged().connect(
        [&](SignalIoConnectionMapItem* item){
            setConnectionMapItem(item); });
}


SignalIoConnectionView::~SignalIoConnectionView()
{
    delete impl;
}


void SignalIoConnectionView::Impl::setConnectionMapItem(SignalIoConnectionMapItem* item)
{
    targetLabel.setText(item ? item->name().c_str() : "---");
}


void SignalIoConnectionView::Impl::onAddButtonClicked()
{

}


void SignalIoConnectionView::Impl::onRemoveButtonClicked()
{

}


bool SignalIoConnectionView::storeState(Archive& archive)
{
    impl->targetItemPicker.storeTargetItem(archive, "currentConnectionMapItem");
    return true;
}


bool SignalIoConnectionView::restoreState(const Archive& archive)
{
    impl->targetItemPicker.restoreTargetItemLater(archive, "currentConnectionMapItem");
    return true;
}