#pragma once
#include <Qt\qobject.h>
#include <qt\qtimer.h>
#include <qtGUI\qapplication.h>
#include <Base\RenderUnit.h>
class YugiohGame : public QObject
{
	Q_OBJECT

	QApplication* containingApp;
	QTimer myTimer;
	RenderUnit renderer;
	bool prepareStartOfGame();
	bool prepareGlobalPointers();
	bool initializeCards();
	bool shutdownCards();
private slots:
	void update();
public:
	void takeAppPointer(QApplication* app);
	void junkFunction();
	bool initialize();
	bool shutdown();
	void go();
};

