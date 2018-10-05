#include "ffflog_omp.h"
FFFLog_omp *global_LogPtr = nullptr;

FFFLog_omp::FFFLog_omp(QWidget *parent) :
   QWidget(parent)
{
   logWidget = new QTextEdit(this);
   layout = new QGridLayout(this);
   layout->addWidget(logWidget);
   logWidget->setReadOnly(true);
   if(!global_LogPtr)
      global_LogPtr = this;
   omp_init_lock(&lock);
}

FFFLog_omp::~FFFLog_omp()
{
}

void FFFLog_omp::logText(std::string logMessage, bool qDebugToo )
{
   if(global_LogPtr){
      global_LogPtr->logTextInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << logMessage.c_str();
   }
}

void FFFLog_omp::logWarning(std::string logMessage, bool qDebugToo)
{

   if(global_LogPtr){
      global_LogPtr->logWarningInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << "WARNING:" << logMessage.c_str();
   }
}

void FFFLog_omp::logError(std::string logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logErrorInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << "ERROR:" << logMessage.c_str();
   }
}

void FFFLog_omp::logText(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logTextInInstance(logMessage);
      if(qDebugToo) qDebug() << logMessage;
   }
}

void FFFLog_omp::logWarning(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logWarningInInstance(logMessage);
      if(qDebugToo) qDebug() << "WARNING:" << logMessage;
   }
}

void FFFLog_omp::logError(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logErrorInInstance(logMessage);
      if(qDebugToo) qDebug() << "ERROR:" << logMessage;
   }
}



void FFFLog_omp::logTextInInstance(QString logMessage)
{
   set_lock();
   logWidget->append(tr("<b>%1\n").arg(logMessage));
   unset_lock();
}

void FFFLog_omp::logWarningInInstance(QString logMessage)
{
   set_lock();
   logWidget->append(tr("<font color=\"#DAA520\"><b>WARNING: %1\n</b></font>").arg(logMessage));
   unset_lock();
}

void FFFLog_omp::logErrorInInstance(QString logMessage)
{
   set_lock();
   logWidget->append(tr("<font color=\"#CC0033\"><b>ERROR: %1\n</b></font>").arg(logMessage));
   unset_lock();
}



void FFFLog_omp::clearLog()
{
   if(global_LogPtr){
      global_LogPtr->clearLogInInstance();
   }
}


void FFFLog_omp::clearLogInInstance()
{
   set_lock();
   logWidget->clear();
   unset_lock();
}
