#include "edit_custom.h"
#include "ui_edit_custom.h"

#include "qv2ray/ui/widgets/editors/w_JsonEditor.hpp"

#include <QInputDialog>

EditCustom::EditCustom(QWidget *parent) :
        QWidget(parent), ui(new Ui::EditCustom) {
    ui->setupUi(this);
    ui->config_simple->setPlaceholderText("example:\n"
                                          "  server-address: \"127.0.0.1:%mapping_port%\"\n"
                                          "  listen-address: \"127.0.0.1\"\n"
                                          "  listen-port: %socks_port%\n"
                                          "  host: your-domain.com\n"
                                          "  sni: your-domain.com\n"
    );
}

EditCustom::~EditCustom() {
    delete ui;
}

void EditCustom::onStart(QSharedPointer<NekoRay::ProxyEntity> _ent) {
    this->ent = _ent;
    auto bean = this->ent->CustomBean();

    P_LOAD_STRING(core)
    ui->command->setText(bean->command.join(" "));
    P_LOAD_STRING(config_simple)

    if (!preset_core.isEmpty()) {
        bean->core = preset_core;
        ui->core->setText(preset_core);
        ui->core->setDisabled(true);
        ui->pick_core->setDisabled(true);
        ui->command->setText(preset_command);
        ui->config_simple->setText(preset_config);
    }
    if (!bean->core.isEmpty()) {
        ui->core->setDisabled(true);
        ui->pick_core->setDisabled(true);
    }

}

bool EditCustom::onEnd() {
    auto bean = this->ent->CustomBean();

    P_SAVE_STRING(core)
    bean->command = ui->command->text().split(" ");
    P_SAVE_STRING_QTEXTEDIT(config_simple)

    return true;
}

void EditCustom::on_pick_core_clicked() {
    QStringList cores;

    auto obj = QString2QJsonObject(NekoRay::dataStore->extraCore->core_map);
    for (const auto &c: obj.keys()) {
        cores << c;
    }

    bool ok;
    auto item = QInputDialog::getItem(this,
                                      QObject::tr("Select"),
                                      QObject::tr("Select a known core"),
                                      cores, 0, false, &ok);

    if (ok) {
        ui->core->setText(item);
    }
}

void EditCustom::on_as_json_clicked() {
    auto editor = new JsonEditor(QString2QJsonObject(ui->config_simple->toPlainText()), this);
    auto result = editor->OpenEditor();
    if (!result.isEmpty()) {
        ui->config_simple->setText(QJsonObject2QString(result, false));
    }
}
