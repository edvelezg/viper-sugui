/*
 * Copyright (c) 2006-2007, Johan Thelin
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,  
 *       this list of conditions and the following disclaimer in the documentation 
 *       and/or other materials provided with the distribution.
 *     * Neither the name of APress nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <QStringList>

#include "listdialog.h"
#include "editdialog.h"
#include "wizard.h"

ListDialog::ListDialog( QWidget *parent ) : QDialog( parent )
{
  
  ui.setupUi( this );
  ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  connect( ui.addButton, SIGNAL(clicked()), this, SLOT(addItem()) );
  connect( ui.editButton, SIGNAL(clicked()), this, SLOT(editItem()) );
  connect( ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()) );
  connect( ui.list, SIGNAL(currentTextChanged ( const QString &)), this, SLOT(enableButton()) );

  ui.list->addItem ( "Modelo 1 -- model.out");
  ui.list->addItem ( "Modelo 2 -- model2.out");
  ui.list->addItem ( "Modelo 3 -- model3.out");
  ui.list->addItem ( "Modelo 4 -- model4.out");

}

void ListDialog::addItem()
{
	ClassWizard *wizard = new ClassWizard( this );
	    wizard->show();
  // EditDialog dlg( this );
  // 
  // if( dlg.exec() == Accepted )
  // 	ui.list->addItem( dlg.modelName() + " -- " + dlg.modelLocation() );
}

void ListDialog::editItem()
{
  if( !ui.list->currentItem() )
    return;
  
  QStringList parts = ui.list->currentItem()->text().split( "--" );
  
  EditDialog dlg( this );
  dlg.setModelName		( parts[0].trimmed() );
  dlg.setModelLocation	( parts[1].trimmed() );
  
  if( dlg.exec() == Accepted )
    ui.list->currentItem()->setText( dlg.modelName() + " -- " + dlg.modelLocation() );
}

void ListDialog::deleteItem()
{
  delete ui.list->currentItem();
}

void ListDialog::enableButton()
{
  ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

const QString ListDialog::currentLocation() const
{
  QStringList parts = ui.list->currentItem()->text().split( "--" );
  return parts[1].trimmed();
}
