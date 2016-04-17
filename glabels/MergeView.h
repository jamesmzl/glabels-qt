/*  MergeView.h
 *
 *  Copyright (C) 2016  Jim Evins <evins@snaught.com>
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

#ifndef MergeView_h
#define MergeView_h

#include "ui_MergeView.h"


class LabelModel;  // Forward reference
	

///
/// Merge Property Editor Widget
///
class MergeView : public QWidget, public Ui_MergeView
{
	Q_OBJECT


	/////////////////////////////////
	// Life Cycle
	/////////////////////////////////
public:
	MergeView( QWidget *parent = 0 );
	~MergeView();


	/////////////////////////////////
	// Public methods
	/////////////////////////////////
	void setModel( LabelModel* model );


	/////////////////////////////////
	// Slots
	/////////////////////////////////
private slots:
	void onLabelChanged();


	/////////////////////////////////
	// Private Data
	/////////////////////////////////
private:
	LabelModel*  mModel;

};


#endif // MergeView_h