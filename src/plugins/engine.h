/****************************************************************************
** $Id: engine.h,v 1.19 2008/05/19 06:45:31 app Exp $
**
** Header file of the Ananas Engine of Ananas
** Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Library of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
** See http://www.leaderit.ru/gpl/ for GPL licensing information.
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

/****************************************************************************
****************************************************************************/
#ifndef ENGINE_H
#define ENGINE_H
#include <qobject.h>
#include <qpopupmenu.h>
#include <qsobjectfactory.h>
#include <qsproject.h>
#include <qsinterpreter.h>

#include "adatabase.h"
#include "acfg.h"
#include "awindowslist.h"
#include <qmainwindow.h>
#include "adatafield.h"
#include "aobject.h"



class aEngine;
class aWidget;

/*!
 * \~english
 *	Register Ananas-engine objects into script engine.
 * \~russian
 *	\brief Метод aObjectsFactory::create регистрирует С++ классы Ананаса в интерпретаторе скриптов,
 *	делая доступным работу с объектами зарегистрированного класса из Ананас.Скрипта.
 *	Наследует QSObjectFactory.
 * \~
 */
class ANANAS_EXPORT aObjectsFactory : public QSObjectFactory
{
public:
        aEngine   *engine;
	aDatabase *db;
        aObjectsFactory( aEngine *e );
        QObject *create( const QString &className,
                     const QSArgumentList &arguments,
                     QObject *context );
};


class aForm;

/*!
 *	\~english
 *	Ananas-engine object.
 *	\~russian
 *	\brief Определяет программный интерфейс Runtime системы, который используется Ананас скриптом.
 *	Наследует QObject.
 *
 *	Класс, реализующий обработку и выполнение скриптов, открытие экранных форм,
 *	предварительную обработку скрипта перед выполнением (для последующего использования русских управляющих инструкций),
 *	системные функции получения даты и времени, печати сообщений об ошибках и т.д
 *	\~
 */
class  ANANAS_EXPORT aEngine : public QObject
{
	Q_OBJECT
public:
/*!
 *	\~english
 *	database connection.
 *	\~russian
 *	Соединение с базой данных.
 *	\~
*/
	aDatabase	db;
/*!
 *	\~english
 *	link to metadata.
 *	\~russian
 *	ссылка на метаданные.
 *	\~
*/
	aCfg*		md;
/*!
 *	\~english
 *	link to workspace.
 *	\~russian
 *	ссылка на рабочее пространство.
 *	\~
*/
	QWidget*	ws;
/*!
 *	\~english
 *	link to windowlist.
 *	\~russian
 *	ссылка на список окон.
 *	\~
*/
	aWindowsList*	wl;
/*!
 *	\~english
 *	link to script interpreter.
 *	\~russian
 *	ссылка на интерпретатор скрипта.
 *	\~
*/
	QSInterpreter*	code;
/*!
 *	\~english
 *	link to script project.
 *	\~russian
 *	ссылка на проект.
 *	\~
*/
	QSProject	project;
/*!
 *	\~english
 *	number to form
 *	\~russian
 *	номер для обеспечения уникальных имен при открытии форм.
 *	\~
*/
	int 		next_obj_id;

	aEngine();
        virtual	~aEngine();
	virtual bool init( const QString &rcfile );
	virtual void done();
	void openEmbedCatalogueEditor(int oid, QWidget *parent=NULL, const bool toSelect=true);

	QString sourcePreprocessor( const QString &src );

	void timerEvent(QTimerEvent *e);
	int on_systemstart();
	int on_systemstop();
	aForm *openForm( int oid, int fid = 0 , int defaultfor = 1,
		int mode = 0, ANANAS_UID id = 0, aWidget* caller = 0, bool modal = false);

public slots:
	QString cfgname();
	QString Time();
	QString Date();
	void Exit();
	void Message(int n, const QString &msg );
	void StatusMessage( const QString &msg );
	void StatusMessage( const QString &msg, const int &pos );
	void settimer(int sec, QString proc);
	aForm * OpenForm(QString fname, int mode=0, aObject * selecter=0, bool modal=false);
	aForm * OpenForm(QString fname, int mode, int ido,aObject * selecter=0, bool modal=false);
	QVariant value( const QString &name );
	void setValue( const QString &name, QVariant value = QVariant::Invalid );

	void on_MenuBar( int id );
	void execAction( aCfgItem &act, QObject *context = 0 );

	aDataField *enterValue( const QString &FieldType, const QString &title = "" );
	bool startTransaction();
	bool commitTransaction();
	bool rollbackTransaction();
private slots:
	void error ( 	const QString & message, QObject * context,
			const QString & scriptName, int lineNumber );
	void on_event( const QString &data );
signals:
        void statusMessage( const QString &msg );
	void statusMessage( const QString &msg, const int &pos );
	void statusIcon( const int &status );
	void event( const QString &source, const QString &data );

private:
	QString pr_timer;
	QString mGlobal;
	QDict <QVariant> values;

};

#endif // ENGINE_H
