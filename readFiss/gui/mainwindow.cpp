#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readFiss.h"

#include <TBrowser.h>

#include <QFileDialog>

#include <sstream>

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f = new readFiss();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete f;
}

void MainWindow::on_Run_clicked()
{
    cout << "\nRunning READ_FAME...\n" << endl;

    ui->centralwidget->setDisabled(true);
    ui->progressBar->setValue(0);
    ui->centralwidget->repaint();

    for(int i = 0; i < NUM_RUNS(); ++i)
    {
      f->SetInfo(this);
      f->Run();
    }

    f->runNum = 0;
    ui->progressBar->setValue(100);
    ui->centralwidget->setDisabled(false);
}


void MainWindow::on_Save_clicked()
{
    cout << "\nSaving...\n" << endl;
    ui->centralwidget->setDisabled(true);

    QString fileName = QFileDialog::getSaveFileName(this, "Save READ_FAME Input", "", "All Files (*)");
    nameInput = fileName.toStdString();
    if(nameInput == "")
    {
      cout << "fileName empty (user cancelled saving)" << endl;
      ui->centralwidget->setDisabled(false);
      return;
    }

    f->Save(this);

    ui->centralwidget->setDisabled(false);
}


void MainWindow::on_Load_clicked()
{
    cout << "\nLoading...\n" << endl;
    ui->centralwidget->setDisabled(true);

    QString fileName = QFileDialog::getOpenFileName(this, "Load READ_FAME Input", "", "All Files (*)");
    nameInput = fileName.toStdString();
    if(nameInput == "")
    {
      cout << "fileName empty (user cancelled loading)" << endl;
      ui->centralwidget->setDisabled(false);
      return;
    }

    f->Load(this);

    ui->centralwidget->setDisabled(false);
}

void MainWindow::on_writeFinder_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save READ_FAME output", "", "ROOT File (*.root);;All Files (*)");
    fileName.remove(".root");
    ui->nameWrite->setText(fileName);
}

void MainWindow::on_expFinder_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Find Experiment File", "", "ROOT File (*.root);;All Files (*)");
    fileName.remove(".root");
    ui->nameExp->setText(fileName);
}


void MainWindow::on_coordFinder_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Find Coordinate File", "", "All Files (*)");
    ui->nameCoords->setText(fileName);
}


void MainWindow::on_simFinder_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Find Simulation File", "", "ROOT File (*.root);;All Files (*)");
    fileName.remove(".root");
    ui->nameSim->setText(fileName);
}


void MainWindow::on_beamFinder_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Find Beam File", "", "ROOT File (*.root);;All Files (*)");
    ui->nameBeam->setText(fileName);
}

void MainWindow::on_openBrowser_clicked()
{
    gROOT->SetBatch(kFALSE);
    cout << gROOT->IsBatch() << endl;
    TBrowser Browser;
}

void MainWindow::on_NUM_RUNS_valueChanged(int arg1)
{
    if(arg1 == 1)
    {
      ui->THRESHOLD2->setEnabled(false);
      ui->CLIPPING2->setEnabled(false);
      ui->MAX_TIME_N2->setEnabled(false);
      ui->THRESHOLD_DEP2->setEnabled(false);
      ui->CLIPPING_DEP2->setEnabled(false);
      ui->BACKGROUND_DELAY2->setEnabled(false);
      ui->FISS_PILEUP_TIME2->setEnabled(false);
      ui->THRESHOLD_Check->setEnabled(false);
      ui->CLIPPING_Check->setEnabled(false);
      ui->MAX_TIME_N_Check->setEnabled(false);
      ui->THRESHOLD_DEP_Check->setEnabled(false);
      ui->CLIPPING_DEP_Check->setEnabled(false);
      ui->BACKGROUND_DELAY_Check->setEnabled(false);
      ui->FISS_PILEUP_TIME_Check->setEnabled(false);
    }
    else
    {
      ui->THRESHOLD2->setEnabled(true);
      ui->CLIPPING2->setEnabled(true);
      ui->MAX_TIME_N2->setEnabled(true);
      ui->THRESHOLD_DEP2->setEnabled(true);
      ui->CLIPPING_DEP2->setEnabled(true);
      ui->BACKGROUND_DELAY2->setEnabled(true);
      ui->FISS_PILEUP_TIME2->setEnabled(true);
      ui->THRESHOLD_Check->setEnabled(true);
      ui->CLIPPING_Check->setEnabled(true);
      ui->MAX_TIME_N_Check->setEnabled(true);
      ui->THRESHOLD_DEP_Check->setEnabled(true);
      ui->CLIPPING_DEP_Check->setEnabled(true);
      ui->BACKGROUND_DELAY_Check->setEnabled(true);
      ui->FISS_PILEUP_TIME_Check->setEnabled(true);
    }
}



// getters/setters
// progress bar, setter only
void MainWindow::setProgress(int value)
{
  ui->progressBar->setValue(value);
}

// inputFile name, getter only
string MainWindow::GetNameInput()
{
  return nameInput;
}

// modes
int MainWindow::mode(int set)
{
  if(set == -1)
  {
    if(ui->simMode->isChecked()) return 1;
    else if(ui->beamMode->isChecked()) return 2;
    else return 0;
  }
  else
  {
    if(set == 1) ui->simMode->setChecked(true);
    else if(set == 2) ui->beamMode->setChecked(true);
    else
    {
      ui->simMode->setChecked(false);
      ui->beamMode->setChecked(false);
    }
    return -1;
  }
}
int MainWindow::NUM_RUNS(int set)
{
  if(set == -1)
  {
    return ui->NUM_RUNS->value();
  }
  else
  {
    ui->NUM_RUNS->setValue(set);
    return -1;
  }
}
bool MainWindow::CovEM_in(int set)
{
  if(set == -1)
  {
    return ui->CovEM_in->isChecked();
  }
  else
  {
    ui->CovEM_in->setChecked((bool)set);
    return -1;
  }
}

// paths
string MainWindow::nameExp(string set)
{
  if(set == "")
  {
    return ui->nameExp->displayText().toStdString();
  }
  else
  {
    ui->nameExp->setText(QString::fromStdString(set));
    return "";
  }
}
string MainWindow::nameSim(string set)
{
  if(set == "")
  {
    return ui->nameSim->displayText().toStdString();
  }
  else
  {
    ui->nameSim->setText(QString::fromStdString(set));
    return "";
  }
}
string MainWindow::nameBeam(string set)
{
  if(set == "")
  {
    return ui->nameBeam->displayText().toStdString();
  }
  else
  {
    ui->nameBeam->setText(QString::fromStdString(set));
    return "";
  }
}
string MainWindow::nameCoords(string set)
{
  if(set == "")
  {
    return ui->nameCoords->displayText().toStdString();
  }
  else
  {
    ui->nameCoords->setText(QString::fromStdString(set));
    return "";
  }
}
string MainWindow::nameWrite(string set)
{
  if(set == "")
  {
    return ui->nameWrite->displayText().toStdString();
  }
  else
  {
    ui->nameWrite->setText(QString::fromStdString(set));
    return "";
  }
}

int MainWindow::numExpFiles(int set)
{
  if(set == -1)
  {
    return ui->numExpFiles->displayText().toInt();
  }
  else
  {
    ui->numExpFiles->setText(QString::number(set));
    return -1;
  }
}
int MainWindow::numSimFiles(int set)
{
  if(set == -1)
  {
    return ui->numSimFiles->displayText().toInt();
  }
  else
  {
    ui->numSimFiles->setText(QString::number(set));
    return -1;
  }
}

// detectors
int MainWindow::NUM_DETECTORS(int set)
{
  if(set == -1)
  {
    return ui->NUM_DETECTORS->displayText().toInt();
  }
  else
  {
    ui->NUM_DETECTORS->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::THRESHOLD(double set)
{
  if(set == -1)
  {
    return ui->THRESHOLD->displayText().toDouble();
  }
  else
  {
    ui->THRESHOLD->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::THRESHOLD2(double set)
{
  if(set == -1)
  {
    if(ui->THRESHOLD_Check->isChecked() && NUM_RUNS() > 1)
    return ui->THRESHOLD2->displayText().toDouble();
    else
    return THRESHOLD();
  }
  else
  {
    ui->THRESHOLD2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::THRESHOLD_Check(int set)
{
  if(set == -1)
  {
    return ui->THRESHOLD_Check->isChecked();
  }
  else
  {
    ui->THRESHOLD_Check->setChecked((bool)set);
    return -1;
  }
}
double MainWindow::CLIPPING(double set)
{
  if(set == -1)
  {
    return ui->CLIPPING->displayText().toDouble();
  }
  else
  {
    ui->CLIPPING->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::CLIPPING2(double set)
{
  if(set == -1)
  {
    if(ui->CLIPPING_Check->isChecked() && NUM_RUNS() > 1)
    return ui->CLIPPING2->displayText().toDouble();
    else
    return CLIPPING();
  }
  else
  {
    ui->CLIPPING2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::CLIPPING_Check(int set)
{
  if(set == -1)
  {
    return ui->CLIPPING_Check->isChecked();
  }
  else
  {
    ui->CLIPPING_Check->setChecked((bool)set);
    return -1;
  }
}
double MainWindow::MAX_TIME_N(double set)
{
  if(set == -1)
  {
    return ui->MAX_TIME_N->displayText().toDouble();
  }
  else
  {
    ui->MAX_TIME_N->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::MAX_TIME_N2(double set)
{
  if(set == -1)
  {
    if(ui->MAX_TIME_N_Check->isChecked() && NUM_RUNS() > 1)
    return ui->MAX_TIME_N2->displayText().toDouble();
    else
    return MAX_TIME_N();
  }
  else
  {
    ui->MAX_TIME_N2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::MAX_TIME_N_Check(int set)
{
  if(set == -1)
  {
    return ui->MAX_TIME_N_Check->isChecked();
  }
  else
  {
    ui->MAX_TIME_N_Check->setChecked((bool)set);
    return -1;
  }
}

// triggers
int MainWindow::NUM_TRIGGERS(int set)
{
  if(set == -1)
  {
    return ui->NUM_TRIGGERS->displayText().toInt();
  }
  else
  {
    ui->NUM_TRIGGERS->setText(QString::number(set));
    return -1;
  }
}
int* MainWindow::TRIGGERS(int* set)
{
  if(set == nullptr)
  {
    int* TRIGGERS = new int[NUM_TRIGGERS()];
    stringstream tIn(ui->TRIGGERS->displayText().toStdString());
    for(int i = 0; i < NUM_TRIGGERS(); ++i)
    {
        tIn >> TRIGGERS[i];
    }
    return TRIGGERS;
  }
  else
  {
    QString tLoad = "";
    for(int i = 0; i < NUM_TRIGGERS(); ++i)
    {
        tLoad += (QString::number(set[i]) + " ");
    }
    ui->TRIGGERS->setText(tLoad);
    return nullptr;
  }
}
double MainWindow::THRESHOLD_DEP(double set)
{
  if(set == -1)
  {
    return ui->THRESHOLD_DEP->displayText().toDouble();
  }
  else
  {
    ui->THRESHOLD_DEP->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::THRESHOLD_DEP2(double set)
{
  if(set == -1)
  {
    if(ui->THRESHOLD_DEP_Check->isChecked() && NUM_RUNS() > 1)
    return ui->THRESHOLD_DEP2->displayText().toDouble();
    else
    return THRESHOLD_DEP();
  }
  else
  {
    ui->THRESHOLD_DEP2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::THRESHOLD_DEP_Check(int set)
{
  if(set == -1)
  {
    return ui->THRESHOLD_DEP_Check->isChecked();
  }
  else
  {
    ui->THRESHOLD_DEP_Check->setChecked((bool)set);
    return -1;
  }
}
double MainWindow::CLIPPING_DEP(double set)
{
  if(set == -1)
  {
    return ui->CLIPPING_DEP->displayText().toDouble();
  }
  else
  {
    ui->CLIPPING_DEP->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::CLIPPING_DEP2(double set)
{
  if(set == -1)
  {
    if(ui->CLIPPING_DEP_Check->isChecked() && NUM_RUNS() > 1)
    return ui->CLIPPING_DEP2->displayText().toDouble();
    else
    return CLIPPING_DEP();
  }
  else
  {
    ui->CLIPPING_DEP2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::CLIPPING_DEP_Check(int set)
{
  if(set == -1)
  {
    return ui->CLIPPING_DEP_Check->isChecked();
  }
  else
  {
    ui->CLIPPING_DEP_Check->setChecked((bool)set);
    return -1;
  }
}

// other
double MainWindow::BACKGROUND_DELAY(double set)
{
  if(set == -1)
  {
    return ui->BACKGROUND_DELAY->displayText().toDouble();
  }
  else
  {
    ui->BACKGROUND_DELAY->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::BACKGROUND_DELAY2(double set)
{
  if(set == -1)
  {
    if(ui->BACKGROUND_DELAY_Check->isChecked() && NUM_RUNS() > 1)
    return ui->BACKGROUND_DELAY2->displayText().toDouble();
    else
    return BACKGROUND_DELAY();
  }
  else
  {
    ui->BACKGROUND_DELAY2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::BACKGROUND_DELAY_Check(int set)
{
  if(set == -1)
  {
    return ui->BACKGROUND_DELAY_Check->isChecked();
  }
  else
  {
    ui->BACKGROUND_DELAY_Check->setChecked((bool)set);
    return -1;
  }
}
double MainWindow::FISS_PILEUP_TIME(double set)
{
  if(set == -1)
  {
    return ui->FISS_PILEUP_TIME->displayText().toDouble();
  }
  else
  {
    ui->FISS_PILEUP_TIME->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::FISS_PILEUP_TIME2(double set)
{
  if(set == -1)
  {
    if(ui->FISS_PILEUP_TIME_Check->isChecked() && NUM_RUNS() > 1)
    return ui->FISS_PILEUP_TIME2->displayText().toDouble();
    else
    return FISS_PILEUP_TIME();
  }
  else
  {
    ui->FISS_PILEUP_TIME2->setText(QString::number(set));
    return -1;
  }
}
bool MainWindow::FISS_PILEUP_TIME_Check(int set)
{
  if(set == -1)
  {
    return ui->FISS_PILEUP_TIME_Check->isChecked();
  }
  else
  {
    ui->FISS_PILEUP_TIME_Check->setChecked((bool)set);
    return -1;
  }
}

// beam settings
double MainWindow::BEAM_ERG_MIN(double set)
{
  if(set == -1)
  {
    return ui->BEAM_ERG_MIN->displayText().toDouble();
  }
  else
  {
    ui->BEAM_ERG_MIN->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::BEAM_ERG_MAX(double set)
{
  if(set == -1)
  {
    return ui->BEAM_ERG_MAX->displayText().toDouble();
  }
  else
  {
    ui->BEAM_ERG_MAX->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::BEAM_ERG_BINNUM(double set)
{
  if(set == -1)
  {
    return ui->BEAM_ERG_BINNUM->displayText().toDouble();
  }
  else
  {
    ui->BEAM_ERG_BINNUM->setText(QString::number(set));
    return -1;
  }
}

// CovEM settings
double MainWindow::MIN_N_ERG(double set)
{
  if(set == -1)
  {
    return ui->MIN_N_ERG->displayText().toDouble();
  }
  else
  {
    ui->MIN_N_ERG->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::MAX_N_ERG(double set)
{
  if(set == -1)
  {
    return ui->MAX_N_ERG->displayText().toDouble();
  }
  else
  {
    ui->MAX_N_ERG->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::MIN_P_ERG(double set)
{
  if(set == -1)
  {
    return ui->MIN_P_ERG->displayText().toDouble();
  }
  else
  {
    ui->MIN_P_ERG->setText(QString::number(set));
    return -1;
  }
}
double MainWindow::MAX_P_ERG(double set)
{
  if(set == -1)
  {
    return ui->MAX_P_ERG->displayText().toDouble();
  }
  else
  {
    ui->MAX_P_ERG->setText(QString::number(set));
    return -1;
  }
}
int MainWindow::BN(int set)
{
  if(set == -1)
  {
    return ui->BN->displayText().toInt();
  }
  else
  {
    ui->BN->setText(QString::number(set));
    return -1;
  }
}
int MainWindow::BP(int set)
{
  if(set == -1)
  {
    return ui->BP->displayText().toInt();
  }
  else
  {
    ui->BP->setText(QString::number(set));
    return -1;
  }
}
