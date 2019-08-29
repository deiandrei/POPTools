/********************************************************************************
** Form generated from reading UI file 'POPToolset.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPTOOLSET_H
#define UI_POPTOOLSET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_POPToolsetClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTreeView *fsTree;
    QHBoxLayout *horizontalLayout;
    QPushButton *loadBtn;
    QLabel *currentFileLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *POPToolsetClass)
    {
        if (POPToolsetClass->objectName().isEmpty())
            POPToolsetClass->setObjectName(QString::fromUtf8("POPToolsetClass"));
        POPToolsetClass->resize(750, 588);
        centralWidget = new QWidget(POPToolsetClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        fsTree = new QTreeView(centralWidget);
        fsTree->setObjectName(QString::fromUtf8("fsTree"));

        verticalLayout->addWidget(fsTree);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        loadBtn = new QPushButton(centralWidget);
        loadBtn->setObjectName(QString::fromUtf8("loadBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(loadBtn->sizePolicy().hasHeightForWidth());
        loadBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(loadBtn);

        currentFileLabel = new QLabel(centralWidget);
        currentFileLabel->setObjectName(QString::fromUtf8("currentFileLabel"));

        horizontalLayout->addWidget(currentFileLabel);


        verticalLayout->addLayout(horizontalLayout);

        POPToolsetClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(POPToolsetClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 750, 26));
        POPToolsetClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(POPToolsetClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        POPToolsetClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(POPToolsetClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        POPToolsetClass->setStatusBar(statusBar);

        retranslateUi(POPToolsetClass);

        QMetaObject::connectSlotsByName(POPToolsetClass);
    } // setupUi

    void retranslateUi(QMainWindow *POPToolsetClass)
    {
        POPToolsetClass->setWindowTitle(QCoreApplication::translate("POPToolsetClass", "POPToolset", nullptr));
        loadBtn->setText(QCoreApplication::translate("POPToolsetClass", "Load File", nullptr));
        currentFileLabel->setText(QCoreApplication::translate("POPToolsetClass", "No file selected", nullptr));
    } // retranslateUi

};

namespace Ui {
    class POPToolsetClass: public Ui_POPToolsetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPTOOLSET_H
