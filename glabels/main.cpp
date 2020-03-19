/*  main.cpp
 *
 *  Copyright (C) 2013-2016  Jim Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"

#include "model/FileUtil.h"
#include "model/Db.h"
#include "model/Model.h"
#include "model/Settings.h"
#include "model/Version.h"
#include "model/XmlLabelParser.h"

#include "barcode/Backends.h"
#include "merge/Factory.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include <QtDebug>

#include <stdio.h>
#include <stdlib.h>

FILE * pFile;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";;
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(pFile, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(pFile, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(pFile, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(pFile, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(pFile, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    }
    fflush (pFile);
}

int main( int argc, char **argv )
{
    const char * fname = QDateTime::currentDateTime().toString("yyyy-MM-dd.log").toStdString().c_str();
    pFile = fopen (fname, "a");
    qInstallMessageHandler(myMessageOutput);
	QApplication app( argc, argv );

	QCoreApplication::setOrganizationName( "glabels.org" );
	QCoreApplication::setOrganizationDomain( "glabels.org" );
	QCoreApplication::setApplicationName( "glabels-qt" );
	QCoreApplication::setApplicationVersion( glabels::model::Version::LONG_STRING );

	//
	// Setup translators
	//
	QLocale locale = QLocale::system();
	QString qtTranslationsDir = QLibraryInfo::location( QLibraryInfo::TranslationsPath );
	QString myTranslationsDir = glabels::model::FileUtil::translationsDir().canonicalPath();
	
	QTranslator qtTranslator;
	if ( qtTranslator.load( locale, "qt", "_", qtTranslationsDir ) )
	{
		app.installTranslator(&qtTranslator);
	}

	QTranslator glabelsTranslator;
	if ( glabelsTranslator.load( locale, "glabels", "_", myTranslationsDir ) )
	{
		app.installTranslator(&glabelsTranslator);
	}

	QTranslator templatesTranslator;
	if ( templatesTranslator.load( locale, "templates", "_", myTranslationsDir ) )
	{
		app.installTranslator(&templatesTranslator);
	}


	//
	// Parse command line
	//
	QCommandLineParser parser;
	parser.setApplicationDescription( QCoreApplication::translate( "main", "gLabels Label Designer" ) );
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument( "files",
	                              QCoreApplication::translate( "main", "gLabels project files to open, optionally." ),
	                              "[files...]" );
	parser.process( app );
	
	//
	// Initialize subsystems
	//
	glabels::model::Settings::init();
	glabels::model::Db::init();
	glabels::merge::Factory::init();
	glabels::barcode::Backends::init();

	
	//
	// Open each file in its own main window
	//
	bool openedFiles = false;
	foreach ( QString filename, parser.positionalArguments() )
	{
		glabels::model::Model *model = glabels::model::XmlLabelParser::readFile( filename );
		if ( model )
		{
			model->setFileName( filename );
			auto *newWindow = new glabels::MainWindow();
			newWindow->setModel( model );
			newWindow->show();
			openedFiles = true;
		}
	}

	
	//
	// Launch main window
	//
	if ( !openedFiles )
	{
		auto *mainWindow = new glabels::MainWindow();
		mainWindow->show();
	}

    qDebug() << "Batch mode.  printer =";
	return app.exec();
}
