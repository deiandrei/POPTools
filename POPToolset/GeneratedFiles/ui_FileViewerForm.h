/********************************************************************************
** Form generated from reading UI file 'FileViewerForm.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEVIEWERFORM_H
#define UI_FILEVIEWERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileViewerForm
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *label1;
    QLabel *nameLabel;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *sizeLabel;
    QLabel *typeLabel;
    QLabel *grsIdLabel;

    void setupUi(QWidget *FileViewerForm)
    {
        if (FileViewerForm->objectName().isEmpty())
            FileViewerForm->setObjectName(QString::fromUtf8("FileViewerForm"));
        FileViewerForm->resize(400, 300);
        gridLayout = new QGridLayout(FileViewerForm);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label1 = new QLabel(FileViewerForm);
        label1->setObjectName(QString::fromUtf8("label1"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label1->setFont(font);
        label1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label1->setIndent(0);

        formLayout->setWidget(0, QFormLayout::LabelRole, label1);

        nameLabel = new QLabel(FileViewerForm);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLabel);

        label = new QLabel(FileViewerForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        label_2 = new QLabel(FileViewerForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(FileViewerForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        sizeLabel = new QLabel(FileViewerForm);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));

        formLayout->setWidget(1, QFormLayout::FieldRole, sizeLabel);

        typeLabel = new QLabel(FileViewerForm);
        typeLabel->setObjectName(QString::fromUtf8("typeLabel"));

        formLayout->setWidget(2, QFormLayout::FieldRole, typeLabel);

        grsIdLabel = new QLabel(FileViewerForm);
        grsIdLabel->setObjectName(QString::fromUtf8("grsIdLabel"));

        formLayout->setWidget(3, QFormLayout::FieldRole, grsIdLabel);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        retranslateUi(FileViewerForm);

        QMetaObject::connectSlotsByName(FileViewerForm);
    } // setupUi

    void retranslateUi(QWidget *FileViewerForm)
    {
        FileViewerForm->setWindowTitle(QCoreApplication::translate("FileViewerForm", "FileViewerForm", nullptr));
        label1->setText(QCoreApplication::translate("FileViewerForm", "<html><head/><body><p>Name</p></body></html>", nullptr));
        nameLabel->setText(QCoreApplication::translate("FileViewerForm", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("FileViewerForm", "Size", nullptr));
        label_2->setText(QCoreApplication::translate("FileViewerForm", "Type", nullptr));
        label_3->setText(QCoreApplication::translate("FileViewerForm", "ResourcesTable Position", nullptr));
        sizeLabel->setText(QCoreApplication::translate("FileViewerForm", "TextLabel", nullptr));
        typeLabel->setText(QCoreApplication::translate("FileViewerForm", "TextLabel", nullptr));
        grsIdLabel->setText(QCoreApplication::translate("FileViewerForm", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileViewerForm: public Ui_FileViewerForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWERFORM_H
