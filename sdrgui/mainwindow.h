///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012 maintech GmbH, Otto-Hahn-Str. 15, 97204 Hoechberg, Germany //
// written by Christian Daniel                                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_MAINWINDOW_H
#define INCLUDE_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QList>
#include <QProcess>

#include "settings/mainsettings.h"
#include "util/message.h"
#include "util/messagequeue.h"
#include "export.h"
#include "mainparser.h"
#include "maincore.h"

class QLabel;
class QTreeWidgetItem;
class QDir;

class DSPEngine;
class DSPDeviceSourceEngine;
class DSPDeviceSinkEngine;
class Indicator;
class GLSpectrumGUI;
class PluginAPI;
class ChannelGUI;
class ChannelMarker;
class PluginManager;
class DeviceAPI;
class DeviceUISet;
class FeatureUISet;
class PluginInterface;
class QWidget;
class WebAPIRequestMapper;
class WebAPIServer;
class WebAPIAdapter;
class Preset;
class Command;
class FeatureSetPreset;
class CommandKeyReceiver;

class QMenuBar;
class Workspace;

// namespace Ui {
// 	class MainWindow;
// }

class SDRGUI_API MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(qtwebapp::LoggerWithFile *logger, const MainParser& parser, QWidget* parent = nullptr);
	~MainWindow();
	static MainWindow *getInstance() { return m_instance; } // Main Window is de facto a singleton so this just returns its reference
	MessageQueue* getInputMessageQueue() { return &m_inputMessageQueue; }
    const PluginManager *getPluginManager() const { return m_pluginManager; }
    std::vector<DeviceUISet*>& getDeviceUISets() { return m_deviceUIs; }
    void commandKeysConnect(QObject *object, const char *slot);
    void commandKeysDisconnect(QObject *object, const char *slot);
    int getNumberOfWorkspaces() const { return m_workspaces.size(); }

private:
    enum {
		PGroup,
		PItem
	};

	struct DeviceWidgetTabData
	{
	    QWidget *gui;
	    QString displayName;
	    QString tabName;
	};

	static MainWindow *m_instance;
    QList<Workspace*> m_workspaces;
    Workspace *m_currentWorkspace;
	// Ui::MainWindow* ui;
	MessageQueue m_inputMessageQueue;
    MainCore *m_mainCore;
	std::vector<DeviceUISet*> m_deviceUIs;
    std::vector<FeatureUISet*> m_featureUIs;
	QList<DeviceWidgetTabData> m_deviceWidgetTabs;

	DSPEngine* m_dspEngine;
	PluginManager* m_pluginManager;

	QTimer m_statusTimer;
	int m_lastEngineState;

    QMenuBar *m_menuBar;
	QLabel* m_dateTimeWidget;
	QLabel* m_showSystemWidget;

	WebAPIRequestMapper *m_requestMapper;
	WebAPIServer *m_apiServer;
	WebAPIAdapter *m_apiAdapter;
	QString m_apiHost;
	int m_apiPort;
	QAction *m_spectrumToggleViewAction;

	CommandKeyReceiver *m_commandKeyReceiver;

	QProcess *m_fftWisdomProcess;

	void loadSettings();
	void loadPresetSettings(const Preset* preset, int tabIndex);
	void savePresetSettings(Preset* preset, int tabIndex);
	void loadFeatureSetPresetSettings(const FeatureSetPreset* preset, int featureSetIndex, Workspace *workspace);
	void saveFeatureSetPresetSettings(FeatureSetPreset* preset, int featureSetIndex);

	QString openGLVersion();
    void createMenuBar();
	void createStatusBar();
	void closeEvent(QCloseEvent*);
	void updatePresetControls();
	QTreeWidgetItem* addPresetToTree(const Preset* preset);
	void applySettings();

    void removeLastDevice();
    void addFeatureSet();
    void removeFeatureSet(unsigned int tabIndex);
    void removeAllFeatureSets();
    void deleteChannel(int deviceSetIndex, int channelIndex);
    void sampleDeviceChange(int deviceType, int deviceSetIndex, int newDeviceIndex, Workspace *workspace);
    void sampleSourceChange(int deviceSetIndex, int newDeviceIndex, Workspace *workspace);
	void sampleSinkChange(int deviceSetIndex, int newDeviceIndex, Workspace *workspace);
	void sampleMIMOChange(int deviceSetIndex, int newDeviceIndex, Workspace *workspace);
    void sampleSourceImplement(
        int deviceSetIndex,
        int deviceIndex,
        DeviceAPI *deviceAPI,
        DeviceUISet *deviceUISet,
        Workspace *workspace
    );
    void sampleSinkImplement(
        int deviceSetIndex,
        int deviceIndex,
        DeviceAPI *deviceAPI,
        DeviceUISet *deviceUISet,
        Workspace *workspace
    );
    void sampleMIMOImplement(
        int deviceSetIndex,
        int deviceIndex,
        DeviceAPI *deviceAPI,
        DeviceUISet *deviceUISet,
        Workspace *workspace
    );
    void deleteFeature(int featureSetIndex, int featureIndex);

    bool handleMessage(const Message& cmd);

private slots:
	void handleMessages();
    void handleWorkspaceVisibility(Workspace *workspace, bool visibility);
	void updateStatus();
    void addWorkspace();
    void viewAllWorkspaces();
    void removeEmptyWorkspaces();
	void loadConfiguration(const Configuration *configuration);
    void saveConfiguration(Configuration *configuration);
	void sampleSourceAdd(Workspace *workspace, int deviceIndex);
	void sampleSinkAdd(Workspace *workspace, int deviceIndex);
	void sampleMIMOAdd(Workspace *workspace, int deviceIndex);
    void sampleDeviceChangeHandler(DeviceGUI *deviceGUI, int newDeviceIndex);

	void on_action_View_Fullscreen_toggled(bool checked);
	void on_presetSave_clicked();
	void on_presetUpdate_clicked();
    void on_presetEdit_clicked();
	void on_presetExport_clicked();
	void on_presetImport_clicked();
	void on_action_saveAll_triggered();
	void on_presetLoad_clicked();
	void on_presetDelete_clicked();
	void on_presetTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_presetTree_itemActivated(QTreeWidgetItem *item, int column);
    void on_action_Configurations_triggered();
	void on_action_Audio_triggered();
    void on_action_Logging_triggered();
	void on_action_FFT_triggered();
    void on_action_AMBE_triggered();
    void on_action_LimeRFE_triggered();
	void on_action_My_Position_triggered();
    void on_action_DeviceUserArguments_triggered();
    void on_action_commands_triggered();
    void channelAddClicked(int channelIndex);
    void featureAddClicked(Workspace *workspace, int featureIndex);
    void featureMove(FeatureGUI *gui, int wsIndexDestnation);
    void openFeaturePresetsDialog(QPoint p, Workspace *workspace);
    void deviceMove(DeviceGUI *gui, int wsIndexDestnation);
    void on_action_Quick_Start_triggered();
    void on_action_Main_Window_triggered();
	void on_action_Loaded_Plugins_triggered();
	void on_action_About_triggered();
	void on_action_removeLastDevice_triggered();
	void on_action_addFeatureSet_triggered();
	void on_action_removeLastFeatureSet_triggered();
	void tabInputViewIndexChanged();
    void tabChannelsIndexChanged();
	void commandKeyPressed(Qt::Key key, Qt::KeyboardModifiers keyModifiers, bool release);
	void fftWisdomProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // INCLUDE_MAINWINDOW_H
