#include "settingswindow.h"
#include "ui_settingswindow.h"

#include "../application.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->settings = Application::getInstance()->settings;

    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->buttonSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(ui->buttonReload, SIGNAL(clicked(bool)), this, SLOT(reloadSettings()));
    connect(ui->shopnameField, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));
    connect(ui->periodCheckField, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->ftpAddressField, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));
    connect(ui->portField, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->usernameField, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));
    connect(ui->passwordField, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));

    reloadSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}


void SettingsWindow::settingsChanged()
{
    ui->buttonSave->setEnabled(true);
    ui->buttonReload->setEnabled(true);
}

void SettingsWindow::saveSettings()
{
    settings->setValue("shopname", ui->shopnameField->text());
    settings->setValue("periodCheck", ui->periodCheckField->value());
    settings->setValue("ftpAddress", ui->ftpAddressField->text());
    settings->setValue("ftpPort", ui->portField->value());
    settings->setValue("ftpUsername", ui->usernameField->text());
    settings->setValue("ftpPassword", ui->passwordField->text());

    ui->buttonSave->setEnabled(false);
    ui->buttonReload->setEnabled(false);
}

void SettingsWindow::reloadSettings()
{
    ui->shopnameField->setText(settings->value("shopname", "").toString());
    ui->periodCheckField->setValue(settings->value("periodCheck", 30).toInt());
    ui->ftpAddressField->setText(settings->value("ftpAddress").toString());
    ui->portField->setValue(settings->value("ftpPort", 21).toInt());
    ui->usernameField->setText(settings->value("ftpUsername").toString());
    ui->passwordField->setText(settings->value("ftpPassword").toString());

    ui->buttonSave->setEnabled(false);
    ui->buttonReload->setEnabled(false);
}
