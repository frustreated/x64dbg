#include "SourceViewerManager.h"
#include "Bridge.h"
#include <QFileInfo>
#include <QDir>

SourceViewerManager::SourceViewerManager(QWidget* parent) : QTabWidget(parent)
{
    setMovable(true);
    setTabsClosable(true);

    //Close All Tabs
    mCloseAllTabs = new QPushButton(this);
    mCloseAllTabs->setIcon(QIcon(":/icons/images/close-all-tabs.png"));
    mCloseAllTabs->setToolTip("Close All Tabs");
    connect(mCloseAllTabs, SIGNAL(clicked()), this, SLOT(closeAllTabs()));
    setCornerWidget(mCloseAllTabs, Qt::TopLeftCorner);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(Bridge::getBridge(), SIGNAL(loadSourceFile(QString, int)), this, SLOT(loadSourceFile(QString, int)));
}

void SourceViewerManager::loadSourceFile(QString path, int line)
{
    for(int i = 0; i < count(); i++) //remove all other instruction pointers (only one is possible)
        ((SourceView*)this->widget(i))->setInstructionPointer(0);
    for(int i = 0; i < count(); i++)
    {
        SourceView* curView = (SourceView*)this->widget(i);
        if(curView->getSourcePath().compare(path, Qt::CaseInsensitive) == 0) //file already loaded
        {
            curView->setInstructionPointer(line);
            setCurrentIndex(i); //show that loaded tab
            return;
        }
    }
    //check if file actually exists
    if(!QFileInfo(path).exists())
    {
        return; //error?
    }
    //load the new file
    QString title = path;
    int idx = path.lastIndexOf(QDir::separator());
    if(idx != -1)
        title = path.mid(idx + 1);
    addTab(new SourceView(path, line), title);
    setCurrentIndex(count() - 1);
}

void SourceViewerManager::closeTab(int index)
{
    removeTab(index);
}

void SourceViewerManager::closeAllTabs()
{
    clear();
}
