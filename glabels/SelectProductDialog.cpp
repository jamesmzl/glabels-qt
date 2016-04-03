/*  SelectProductDialog.cpp
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

#include "SelectProductDialog.h"

#include "libglabels/Db.h"
#include "TemplatePickerItem.h"
#include "Settings.h"

#include <QtDebug>


///
/// Constructor
///
SelectProductDialog::SelectProductDialog( QWidget *parent )
	: QDialog(parent), mCanceled(false)
{
	setupUi( this );

	pageSizeIsoCheck->setChecked( Settings::searchIsoPaperSizes() );
	pageSizeUsCheck->setChecked( Settings::searchUsPaperSizes() );
	pageSizeOtherCheck->setChecked( Settings::searchOtherPaperSizes() );

	anyCategoryCheck->setChecked( Settings::searchAllCategories() );
	mCategoryIdList = Settings::searchCategoryList();

	QList<glabels::Category*> categories = glabels::Db::categories();
	foreach ( glabels::Category *category, categories )
	{
		QCheckBox* check = new QCheckBox( category->name() );
		check->setChecked( mCategoryIdList.contains( category->id() ) || anyCategoryCheck->isChecked() );
		categoriesLayout->addWidget( check );

		mCheckToCategoryMap[check] = category->id();

		connect( check, SIGNAL(clicked()), this, SLOT(onCategoryCheckClicked()) );
	}

	QList<glabels::Template*> tmplates = glabels::Db::templates();
	templatePicker->setTemplates( tmplates );

	templatePicker->applyFilter( searchEntry->text(),
				     pageSizeIsoCheck->isChecked(),
				     pageSizeUsCheck->isChecked(),
				     pageSizeOtherCheck->isChecked(),
				     anyCategoryCheck->isChecked(),
				     mCategoryIdList );
}

///
/// Get selected template
///
const glabels::Template* SelectProductDialog::tmplate() const
{
	if ( !mCanceled )
	{
		return templatePicker->selectedTemplate();
	}
	else
	{
		return 0;
	}
}


///
/// Search Entry Text Changed Slot
///
void SelectProductDialog::onSearchEntryTextChanged()
{
	templatePicker->applyFilter( searchEntry->text(),
				     pageSizeIsoCheck->isChecked(),
				     pageSizeUsCheck->isChecked(),
				     pageSizeOtherCheck->isChecked(),
				     anyCategoryCheck->isChecked(),
				     mCategoryIdList );
}


///
/// Search Entry Text Changed Slot
///
void SelectProductDialog::onSearchClearButtonClicked()
{
	searchEntry->setText( "" );
}


///
/// Page Size Check Clicked Slot
///
void SelectProductDialog::onPageSizeCheckClicked()
{
	Settings::setSearchIsoPaperSizes( pageSizeIsoCheck->isChecked() );
	Settings::setSearchUsPaperSizes( pageSizeUsCheck->isChecked() );
	Settings::setSearchOtherPaperSizes( pageSizeOtherCheck->isChecked() );

	templatePicker->applyFilter( searchEntry->text(),
				     pageSizeIsoCheck->isChecked(),
				     pageSizeUsCheck->isChecked(),
				     pageSizeOtherCheck->isChecked(),
				     anyCategoryCheck->isChecked(),
				     mCategoryIdList );
}


///
/// Any category Check Clicked Slot
///
void SelectProductDialog::onAnyCategoryCheckClicked()
{
	if ( anyCategoryCheck->isChecked() )
	{
		foreach( QCheckBox* check, mCheckToCategoryMap.keys() )
		{
			check->setChecked( true );
		}
	}
	
	loadCategoryList();

	templatePicker->applyFilter( searchEntry->text(),
				     pageSizeIsoCheck->isChecked(),
				     pageSizeUsCheck->isChecked(),
				     pageSizeOtherCheck->isChecked(),
				     anyCategoryCheck->isChecked(),
				     mCategoryIdList );
}


///
/// Category Check Clicked Slot
///
void SelectProductDialog::onCategoryCheckClicked()
{
	bool allFlag = true;
	foreach( QCheckBox* check, mCheckToCategoryMap.keys() )
	{
		allFlag = allFlag && check->isChecked();
	}
	anyCategoryCheck->setChecked( allFlag );

	loadCategoryList();

	templatePicker->applyFilter( searchEntry->text(),
				     pageSizeIsoCheck->isChecked(),
				     pageSizeUsCheck->isChecked(),
				     pageSizeOtherCheck->isChecked(),
				     anyCategoryCheck->isChecked(),
				     mCategoryIdList );
}


///
/// Template Picker Selection Changed Slot
///
void SelectProductDialog::onTemplatePickerSelectionChanged()
{
	// Delay close.  This should make the selection more apparent to the user.
	mTimer.start( 125, this );
}


///
/// Cancel Button Clicked Slot
///
void SelectProductDialog::onCancelButtonClicked()
{
	mCanceled = true;
	close();
}


///
/// Cancel Button Clicked Slot
///
void SelectProductDialog::timerEvent( QTimerEvent *event )
{
	mTimer.stop();
	close();
}


///
/// Load category list
///
void SelectProductDialog::loadCategoryList()
{
	mCategoryIdList.clear();

	foreach( QCheckBox* check, mCheckToCategoryMap.keys() )
	{
		if ( check->isChecked() )
		{
			mCategoryIdList.append( mCheckToCategoryMap[check] );
		}
	}

	Settings::setSearchAllCategories( anyCategoryCheck->isChecked() );
	Settings::setSearchCategoryList( mCategoryIdList );
}