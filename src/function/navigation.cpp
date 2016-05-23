/****************************************************************************

 - Codename: CHER-Ob (Yale Computer Graphics Group)

 - Writers:  Weiqi Shi (weiqi.shi@yale.edu)

 - License:  GNU General Public License Usage
   Alternatively, this file may be used under the terms of the GNU General
   Public License version 3.0 as published by the Free Software Foundation
   and appearing in the file LICENSE.GPL included in the packaging of this
   file. Please review the following information to ensure the GNU General
   Public License version 3.0 requirements will be met:
   http://www.gnu.org/copyleft/gpl.html.

 - Warranty: This software is distributed WITHOUT ANY WARRANTY; without even
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.

 - Acknowledgments: Some portions of this file are based on the example codes
   of ITK/VTK library from Kitware, QT API from Nokia. I would like to thank
   anonymous help from various software engineering communities.

   This file defines the different classes of bookmark that the user may work
   with (corresponding to the different file types that may be displayed).

*****************************************************************************/

#include "navigation.h"
#include "../information/informationWidget.h"
#include "../mainWindow.h"
#include <QFileIconProvider>

Navigation::Navigation()
{
	mTreeWidget = new QTreeWidget();
	QStringList ColumnNames;
	ColumnNames << "Navigation";
	mTreeWidget->setHeaderLabels(ColumnNames);
	connect(mw()->mInformation, SIGNAL(addNavigationItem(const QString, const QString)), this, SLOT(addNoteItem(const QString, const QString)));
	connect(mw()->mInformation, SIGNAL(removeNavigationItem(const QString, const QString, const int)), this, SLOT(removeNoteItem(const QString, const QString, const int)));
	connect(mTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showTreeWidgetItem(QTreeWidgetItem*, int)));
	mVBox = new QVBoxLayout();
	mVBox->addWidget(mTreeWidget);
	this->setLayout(mVBox);
	this->show();
}

void Navigation::init(const QString name, bool isProject)
{
	qDebug()<<"in init";
	clear();

	mName = name;
	QTreeWidgetItem *itm = new QTreeWidgetItem();
	isCurrentProject = isProject;
	if (isProject)
	{
		itm->setText(0, QString("Project: ").append(name));
		itm->setIcon(0, QIcon(":/icons/CHEROb_Logo128.png"));
	}
	else
	{
		QFileIconProvider iconProvider;
		itm->setText(0, QString("Cultural Heritage Entity: ").append(name));
		itm->setIcon(0, iconProvider.icon(QFileIconProvider::Drive));
	}
	
	mItems.push_back(itm);
	mTreeWidget->insertTopLevelItems(0, mItems);
}

void Navigation::clear()
{
	mItems.clear();
	mTreeWidget->clear();
	mName = QString();
}

void Navigation::addObject(const QString path, const QString CHE)
{
	//qDebug()<<"add object"<<path<<CHE;
	QString localPath = QDir::toNativeSeparators(path);
	QStringList nameElements = localPath.split(QDir::separator());
	QString objectName = nameElements[nameElements.size() - 1];
	QTreeWidgetItem *object = new QTreeWidgetItem();
	object->setText(0, objectName);
	QFileIconProvider iconProvider;
	object->setIcon(0, iconProvider.icon(QFileIconProvider::Folder));

	if (CHE != QString() && isCurrentProject)
	{
		bool isFound = false;
		int i = 0;
		for (; i < mItems[0]->childCount(); i++)
		{
			if (mItems[0]->child(i)->text(0) == CHE)
			{
				isFound = true;
				break;
			}
		}
		if (isFound)
		{
			mItems[0]->child(i)->addChild(object);
		}
		else
		{
			QTreeWidgetItem *cheItem = new QTreeWidgetItem();
			cheItem->setText(0, QString("Cultural Heritage Entity: ").append(CHE));
			cheItem->setIcon(0, iconProvider.icon(QFileIconProvider::Drive));
			mItems[0]->addChild(cheItem);
			cheItem->addChild(object);
		}
	}
	else
	{
		mItems[0]->addChild(object);
	}
	loadObjectNote(object, localPath);
	loadObjectBookMark(object, localPath);
	mTreeWidget->expandAll();
}

void Navigation::loadObjectNote(QTreeWidgetItem *object, const QString path)
{
	QFileIconProvider iconProvider;
	QTreeWidgetItem *noteLabel = new QTreeWidgetItem();
	noteLabel->setText(0, QString("Note"));
	noteLabel->setIcon(0, iconProvider.icon(QFileIconProvider::Folder));
	object->addChild(noteLabel);
	QVector<int> noteCount = mw()->mInformation->getNoteNumber(path);	// no notes are marked as removed
	if (noteCount.size() == 6)
	{
		for (int i = 0; i < noteCount.size(); i++)
		{
			if (noteCount[i] == 0)
				continue;
			QString label;
			switch(i)
			{
				case 0: label = QString("Annotation"); break;
				case 1: label = QString("PointNote "); break;
				case 2: label = QString("SurfaceNote "); break;
				case 3: label = QString("FrusumNote "); break;
				case 4: label = QString("PointNote "); break;
				case 5: label = QString("SurfaceNote "); break;
			}
			if (i == 0)
			{
				QTreeWidgetItem *note = new QTreeWidgetItem();
				QString text = label;
				note->setText(0, text);
				note->setText(1, path);
				note->setIcon(0, iconProvider.icon(QFileIconProvider::File));
				noteLabel->addChild(note);
			}
			else 
			{
				for (int j = 0; j < noteCount[i]; j++)
				{
					QTreeWidgetItem *note = new QTreeWidgetItem();
					QString text = label;
					note->setText(0, text.append(QString::number(j+1)));
					note->setText(1, path);
					note->setIcon(0, iconProvider.icon(QFileIconProvider::File));
					noteLabel->addChild(note);
				}
			}
		}
	}
}

void Navigation::loadObjectBookMark(QTreeWidgetItem *object, const QString path)
{
	QTreeWidgetItem *bookmark = new QTreeWidgetItem();
	bookmark->setText(0, QString("BookMark"));
	QFileIconProvider iconProvider;
	bookmark->setIcon(0, iconProvider.icon(QFileIconProvider::Folder));
	object->addChild(bookmark);
}

void Navigation::removeObject(const QString name)
{
}

void Navigation::showTreeWidgetItem(QTreeWidgetItem* item, int column)
{
	if (item->columnCount() == 2)	// check the item is note or not
		mw()->mInformation->openNoteFromNavigation(item);
}

void Navigation::addNoteItem(const QString path, const QString type)
{
	// path shoule be in the form of path/object/Note
	QStringList elements = path.split(QDir::separator());
	QString fileName = elements[elements.size()-2];
	QString objectPath = path;
	objectPath.truncate(objectPath.lastIndexOf(QDir::separator()));
	bool isFound = false;
	QTreeWidgetItem* object;
	for (int i = 0; i < mItems[0]->childCount(); i++)
	{
		QString text = mItems[0]->child(i)->text(0);
		if (text.split(": ")[0] == QString("Cultural Heritage Entity"))	//should use icon
		{
			for (int j = 0; j < mItems[0]->child(i)->childCount(); j++)
			{
				if (mItems[0]->child(i)->child(j)->text(0) == fileName)
				{
					object = mItems[0]->child(i)->child(j)->child(0);
					isFound = true;
					break;
				}
			}
		}
		else
		{
			if (text == fileName)
			{
				object = mItems[0]->child(i)->child(0);
				isFound = true;
			}
		}
		if (isFound)
			break;
	}

	if (isFound)
	{
		int point = 0, surface = 0, frustum = 0;
		for (int k = 0; k < object->childCount(); k++)
		{
			if (object->child(k)->text(0).split(" ")[0] == QString("PointNote"))
				point++;
			else if (object->child(k)->text(0).split(" ")[0] == QString("SurfaceNote"))
				surface++;
			else if (object->child(k)->text(0).split(" ")[0] == QString("FrustumNote"))
				frustum++;
		}
		QFileIconProvider iconProvider;
		QTreeWidgetItem* newNote = new QTreeWidgetItem();
		newNote->setIcon(0, iconProvider.icon(QFileIconProvider::File));
		if (type == QString("Annotation"))
		{
			newNote->setText(0, QString("Annotation"));
			newNote->setText(1, objectPath);
			if (object->childCount() == 0)
				object->addChild(newNote);
			else if (object->child(0)->text(0) != QString("Annotation"))
				object->insertChild(0, newNote);
		}
		if (type == QString("PointNote"))
		{
			newNote->setText(0, QString("PointNote ").append(QString::number(point+1)));
			newNote->setText(1, objectPath);
			if (object->childCount() == 0)
				object->addChild(newNote);
			else if (object->child(0)->text(0) == QString("Annotation"))
				object->insertChild(point+1, newNote);
			else
				object->insertChild(point, newNote);
		}
		else if (type == QString("SurfaceNote"))
		{
			newNote->setText(0, QString("SurfaceNote ").append(QString::number(surface+1)));
			if (object->childCount() == 0)
				object->addChild(newNote);
			else if (object->child(0)->text(0) == QString("Annotation"))
				object->insertChild(point+surface+1, newNote);
			else
				object->insertChild(point+surface, newNote);
		}
		else if (type == QString("FrustumNote "))
		{
			newNote->setText(0, QString("FrustumNote").append(QString::number(frustum+1)));
			if (object->childCount() == 0)
				object->addChild(newNote);
			else if (object->child(0)->text(0) == QString("Annotation"))
				object->insertChild(point+surface+frustum+1, newNote);
			else
				object->insertChild(point+surface+frustum, newNote);
		}
	}
}

void Navigation::removeNoteItem(const QString path, const QString type, const int id)
{
}

MainWindow* Navigation::mw()
{
  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    MainWindow* mainwindow = dynamic_cast<MainWindow*>(widget);
    if (mainwindow)
    {
      return mainwindow;
    }
  }
}
