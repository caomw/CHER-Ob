/****************************************************************************

 - Codename: CHER-Ob (Yale Computer Graphics Group)

 - Writers:   Min H. Kim (minhkim@cs.yale.edu)

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
   annonymous help by various software engineering communities.

*****************************************************************************/
#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QToolBar>
#include <QTextBrowser>
#include <QVector>
#include <vtkPlanes.h>
#include <vtkPlane.h>
#include <QTabWidget>
#include "note.h"

// Class list
class MainWindow;

// DT: DTextEdit class extends QTextEdit to provide basic rich text editing capabilities.

class DTextEdit : public QWidget
{
    Q_OBJECT

public:
    DTextEdit(QWidget *parent = 0);
    ~DTextEdit();

private:

    QAction *actionSave;
    QAction *actionTextBold;
    QAction *actionTextUnderline,
    *actionTextItalic,
    *actionTextColor,
    *actionAlignLeft,
    *actionAlignCenter,
    *actionAlignRight,
    *actionAlignJustify,
    *actionUndo,
    *actionRedo,
    *actionCut,
    *actionCopy,
    *actionPaste;

  QComboBox *comboStyle;
  QFontComboBox *comboFont;
  QComboBox *comboSize;

  QToolBar *tb;
  QString fileName;
  QTextEdit *textEdit;
};


class Information : public QWidget
{
  Q_OBJECT

public:
  Information(QWidget *parent = 0);
  ~Information();

  static MainWindow* mw(); // find the mainframe

  void refresh();

  void initAnnotation(const QString path);

  void startAnnotation();

  void finishAnnotation();

  void reloadAnnotation();

  void clearAnnotation();

  void saveAllNotes();

  void saveObjectNotes();

  void removeAllNotes();

  void removeAllNotes(QString path);

  void removeUnSavedNotes();

  void closeAllNotes();

  void closeObjectNotes();

  void createPointNote(double *pos, int cellId, ColorType color);

  void createSurfaceNote(vtkSmartPointer<vtkSelectionNode> cellIds, ColorType color);

  void createFrustumNote(vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDataArray> normals, ColorType color);

  void createPointNote2D(double* point, int* pointImage, ColorType color);	// world coordinate, image coordinate, color

  void createSurfaceNote2D(double* point, int* pointImage, ColorType color);// world coordinate, image coordinate, color

  void openPointNote(int cellId);

  void openSurfaceNote(vtkSmartPointer<vtkSelectionNode> cellIds);

  void openFrustumNote(vtkSmartPointer<vtkPlanes> planes);

  void openPointNote2D(double* point);

  void openSurfaceNote2D(double* point);

  void openNoteFromTreeWidget(QTreeWidgetItem* item);

  void openNoteFromNavigation(QTreeWidgetItem* item);

  bool checkAllSaved();

  bool checkObjectSaved();

  QVector<QString> getAllNotes(const QString objectPath);

  // return the number of notes for each note type (annotation, point, surface, frustm, point2d, surface2d)for navigation
  QVector<int> getNoteNumber(const QString objectPath);	

signals:
  void saveAll();

  void closeAll();

  void updateMenu();

  void addNavigationItem(const QString path, const QString type);

  void removeNavigationItem(const QString path, const QString type, const int id);

public slots:

  void removePointNote(int noteId, QString* path);

  void removeSurfaceNote(int noteId, QString* path);

  void removeFrustumNote(int noteId, QString* path);

  void removePointNote2D(int noteId, QString* path);

  void removeSurfaceNote2D(int noteId, QString* path);

private:

  bool loadPointNote(const QString path);

  bool loadSurfaceNote(const QString path);

  bool loadFrustumNote(const QString path);

  bool loadPointNote2D(const QString path);

  bool loadSurfaceNote2D(const QString path);

  void hideNotes();

  void showNotes();

  bool updateCurrentPath();

private slots:

  void annotationChanges();

  void saveAnnotation();

  void removeAnnotation();

private:
  
  QHBoxLayout* hlay1;
  QHBoxLayout* hlay2;
  QVBoxLayout* vlay;
  QTextBrowser* dText;
  QPushButton* saveButton;
  QPushButton* removeButton;
  QString notePath;
  QMap<QString, std::pair<QString, bool > > content;	// Annotation
  QMap<QString, QVector<PointNote*>> mPointNotes;
  QMap<QString, QVector<SurfaceNote*>> mSurfaceNotes;
  QMap<QString, QVector<FrustumNote*>> mFrustumNotes;
  QMap<QString, QVector<PointNote2D*>> mPointNotes2D;
  QMap<QString, QVector<SurfaceNote2D*>> mSurfaceNotes2D;
  QMap<QString, bool> hasNotesRemoved;
  bool skipTextChange;
};
#endif // INFORMATION_H

