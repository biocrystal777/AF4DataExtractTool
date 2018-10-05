#include "mainwindow.h"

using std::vector;
using std::string;
using std::list;

mainWindow::mainWindow(QWidget *parent) :
   QWidget(parent)
{
   QScreen *screen = QGuiApplication::primaryScreen();
   QRect  screenGeometry = screen->geometry();
   const int screenHeight = screenGeometry.height();
   const int screenWide = screenGeometry.width();
   thisLay = new QGridLayout(this);

   fileWidget = new FFFFileInOutWidget(QString("mainWindow"), "_mw", this);
   thisLay->addWidget(fileWidget, 0, 0, 1, 15);

   /*
   QGroupBox    *sepMethodBox        = nullptr;
   QVBoxLayout  *sepMethodBoxLay     = nullptr;
   QRadioButton *derivativeSepBut    = nullptr;

QRadioButton *thresholdSepBut     = nullptr;

    QFrame       *derivativeSep       = nullptr;
    QGridLayout *derivativeSepLay     = nullptr;
   */
   sepMethodBox     = new QGroupBox(QString("Separation Method"),this);
   sepMethodBoxLay  = new QGridLayout(sepMethodBox);
   derivativeSepBut = new QRadioButton("Derivative", sepMethodBox);
   sepMethodBoxLay->addWidget(derivativeSepBut, 0,0,3,1);
   derivativeSepFrame = new QFrame(sepMethodBox);
   derivativeSepFrame->setFrameStyle(0x1011);
   sepMethodBoxLay->addWidget(derivativeSepFrame,0,3,3,5);

   thresholdSepBut = new QRadioButton("Threshold", sepMethodBox);
   sepMethodBoxLay->addWidget(thresholdSepBut, 3,0,3,1);
   thresholdSepFrame = new QFrame(sepMethodBox);
   thresholdSepFrame->setFrameStyle(0x1011);
   sepMethodBoxLay->addWidget(thresholdSepFrame, 3,3,3,5);

   sepMethodBox->setDisabled(true);

   thisLay->addWidget(sepMethodBox, 3, 0, 5, 15);


   useThreshold = new QCheckBox(this);
   thisLay->addWidget(useThreshold, 9, 0);

   thisLay->addWidget(new QLabel("Threshold", this), 9, 1, Qt::AlignRight);
   peakThreshold = new QDoubleSpinBox(this);
   peakThreshold->setDecimals(4);
   peakThreshold->setMinimum(0.00);
   peakThreshold->setMaximum(100.0);
   thisLay->addWidget(peakThreshold, 9, 2, Qt::AlignLeft);
   QObject::connect(useThreshold, SIGNAL(toggled(bool)), peakThreshold, SLOT(setEnabled(bool)));
   useThreshold->setChecked(true);
   thisLay->addWidget(new QLabel("E-7", this), 9, 3, Qt::AlignLeft);


   thisLay->addWidget(new QLabel("min. peak width:", this), 10, 0, Qt::AlignRight);
   minPeakWidth = new QSpinBox(this);
   minPeakWidth->setMinimum(0);
   minPeakWidth->setMaximum(1000000);
   thisLay->addWidget(minPeakWidth, 10, 2, Qt::AlignLeft);

   thisLay->addWidget(new QLabel("min. distance width:" ,this), 10, 3, Qt::AlignRight);
   minPeakDistance = new QSpinBox(this);
   minPeakDistance->setMinimum(0);
   minPeakDistance->setMaximum(1000000);
   thisLay->addWidget(minPeakDistance, 10, 5, Qt::AlignLeft);

   thisLay->addWidget(new QLabel("Peak top half width:" ,this), 10, 6, Qt::AlignRight);
   minPeakTopWidth = new QSpinBox(this);
   minPeakTopWidth->setMinimum(1);
   minPeakTopWidth->setMaximum(1000000);
   thisLay->addWidget(minPeakTopWidth, 10, 8, Qt::AlignLeft);

   start = new QPushButton(tr("ExtractMw"),this);
   QObject::connect(start, SIGNAL(pressed()), this, SLOT(startExtraction()));
   thisLay->addWidget(start, 10, 9);

   logWidget = new FFFLog_omp(this);
   thisLay->addWidget(logWidget, 21, 0, 10, 15);

   this->setMinimumSize(screenWide/4, screenHeight/4);
   this->setMaximumSize(screenWide/2, screenHeight/2);

   loadParameters();
}

void mainWindow::loadParameters()
{
   QSettings settings("AgCoelfen", "ExtractTool");
   settings.setIniCodec("UTF-8");
   //int initInt(0);
   //qDebug() << "X" << settings.value(tr("mxPosX"), 0).toInt();
   this->move(settings.value(tr("mwPosX"), 0).toInt(),
              settings.value(tr("mwPosY"), 0).toInt());
   this->resize(settings.value(tr("mwWidth"),  this->minimumWidth()  ).toInt(),
                settings.value(tr("mwHeight"), this->minimumHeight() ).toInt());
   this->minPeakWidth->setValue(  settings.value(tr("minPeakWidth"), 0).toUInt() );
   this->minPeakDistance->setValue(  settings.value(tr("minPeakDistance"), 0).toUInt() );
   this->minPeakTopWidth->setValue(  settings.value(tr("minPeakTopWidth"), 1).toUInt() );
   this->peakThreshold->setValue(  settings.value(tr("peakThreshold"), 1).toDouble() );



   // Alternative Macro for loading parameters of type int and
   // using the identifier of a setting function fo the initialization
   /*
 #ifndef INIT_PARAMETER_VARSETFUNC_INT
 #define INIT_PARAMETER_VARSETFUNC_INT(keyName, varSetFunc, defaultValue){ \
    initInt = settings.value(tr(keyName), defaultValue).toDouble(&ok); \
    if(!ok){ \
    FFFLog_omp::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
    .arg(keyName).arg(defaultValue)); \
 }\
    varSetFunc(initInt); \
 };

#endif //INIT_PARAMETER_VARSETFUNC

    //INIT_PARAMETER_VARSETFUNC_INT("mwPosx";this->resize();, this->minimumSize().width());
*/

   //#undef INIT_PARAMETER_VARSETFUNC_INT
}

void mainWindow::saveParameters()
{
   QSettings settings("AgCoelfen", "ExtractTool");
   settings.setIniCodec("UTF-8");

   settings.setValue(tr("mwPosX"),          this->pos().x()                 );
   settings.setValue(tr("mwPosY"),          this->pos().y()                 );
   settings.setValue(tr("mwWidth"),         this->width()                   );
   settings.setValue(tr("mwHeight"),        this->height()                  );
   settings.setValue(tr("peakThreshold"),   this->peakThreshold->value()    );
   settings.setValue(tr("minPeakWidth"),    this->minPeakWidth->value()     );
   settings.setValue(tr("minPeakDistance"), this->minPeakDistance->value()  );
   settings.setValue(tr("minPeakTopWidth"), this->minPeakTopWidth->value()  );
}

void mainWindow::startExtraction()
{
   qDebug() << 1;

   QString inFileName = fileWidget->getInputFilePath();
   if(inFileName.isEmpty()){
      FFFLog_omp::logWarning(tr("No input file specified!"));
      return;
   }
   qDebug() << 1;
   FFFLog_omp::logText(tr("Read data from %1").arg(inFileName));
   QString outFileNameTemplate = inFileName;
   outFileNameTemplate.remove(inFileName.size()-4, inFileName.size()-1);
  qDebug() << 2;
   FFFCsvParser parser(inFileName.toStdString(), ',', '.');
   parser.parseFile();
   qDebug() << 25;
   matD data = parser.getData();
   vector<string> headLines = parser.getHeadLines();
  qDebug() << 3;
   QFile parLogFile(QString(outFileNameTemplate).append("_pars.csv"));
   if(!parLogFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
      FFFLog_omp::logError(tr("Could not write results to %1").arg("_pars.txt"));
      return;
   }
   QTextStream parLog(&parLogFile);
   qDebug() << 2;

   const bool useThres = useThreshold->isChecked();
   const double threshold = peakThreshold->value() * 1.0e-7;
   const uint minPWidth = static_cast<uint>(minPeakWidth->value());
   const uint minPDist  = static_cast<uint>(minPeakDistance->value());
   const uint peakTopWidth = static_cast<uint>(minPeakTopWidth->value());
   //const vecD Ds         = data[1];
   vecD MWs              = data[2];
   const vecD Rh         = data[4];
   const vecD RI         = data[6];
   const uint dataLength = MWs.size();

   // 1st step: find all data sets between "zero-islands"

   list<uint> peakCandStarts;
   list<uint> peakCandEnds;

   FFFLog_omp::logText(tr("Start extraction using following parameters:"));
   if(useThres){
      FFFLog_omp::logText(tr("Check for \"Mw = 0 entries\" and use threshold of RI to identify peaks"));
      FFFLog_omp::logText(tr("Threshold: %1").arg(threshold));
      parLog << tr("Used additional RI threshold;") << tr("true") << endl;
      parLog << tr("Threshold;") << threshold << endl;
      // mark Mws with corresponding Ris under threshold:
      for(uint i = 0; i < dataLength; ++i)
         if(RI[i] < threshold) MWs[i] = 0.0;
   }
   else {
      FFFLog_omp::logText(tr("Check only for \"Mw = 0 entries\" to identify peaks."));
      parLog << tr("Used additional RI threshold;") << tr("false") << endl;
   }
   FFFLog_omp::logText(tr("min. peak width: %1").arg(minPWidth));
   parLog << tr("min. peak width;") << minPWidth << endl;
   FFFLog_omp::logText(tr("min. distance width: %1").arg(minPDist));
   parLog << tr("min. distance width;") << minPDist << endl;
   FFFLog_omp::logText(tr("Peak top half width: %1").arg(peakTopWidth));
   parLog << tr("Peak top half width;") << peakTopWidth << endl;

   FFFLog_omp::logText(tr("Identify start and end points of data scopes.."));

   // look for the first start:
   for(uint i = 0; i <  dataLength-1; ++i){
      if(isZero(MWs[i]) && !isZero(MWs[i+1])){ // 0.0  0.0  0.0  5.3
         peakCandStarts.push_back(i+1);
         break;
      }
   }
   qDebug() << 3;

   if(peakCandStarts.size() == 0){

      FFFLog_omp::logText(tr("No candidate scopes identified. (|s| = %1)").arg(peakCandStarts.size()));
      parLog << tr("Identified candidate peaks;0") << endl;
      parLogFile.close();
      return;
   }

   qDebug() << 31;

   // now look for starts AND ends:
   for(uint i = peakCandStarts.front(); i < dataLength-1; ++i){
      if(isZero(MWs[i]) && !isZero(MWs[i+1])){ // 0.0  0.0  0.0  5.3
         peakCandStarts.push_back(i+1);
      }
      if(!isZero(MWs[i]) && isZero(MWs[i+1])) // 5.4  1.7  2.9  0.0
         peakCandEnds.push_back(i);
   }

  qDebug() << 32;

   if(peakCandEnds.size() == 0){
      FFFLog_omp::logText(tr("No candidate scopes identified.(|s| = %1, |e| = %2)").arg(peakCandStarts.size()).arg (peakCandEnds.size()));
      FFFLog_omp::logText(tr("No candidate ends identified."));
      parLog << tr("Identified candidate scopes;0") << endl;
      parLogFile.close();
      return;
   }

   qDebug() << 33;

   if(peakCandEnds.size() == peakCandStarts.size() + 1){
      FFFLog_omp::logText(tr("Skip data tail."));
      peakCandStarts.pop_back();
      FFFLog_omp::logText(tr("Identified %1 candidates.").arg(peakCandStarts.size()));
      parLog << tr("Identified candidate peaks;") << peakCandStarts.size() << endl;
   }
   else if (peakCandEnds.size() == peakCandStarts.size()){
      FFFLog_omp::logText(tr("Identified %1 candidates.").arg(peakCandStarts.size()));
      parLog << tr("Identified candidate peaks;") << peakCandStarts.size() << endl;
   }

   qDebug() << 34;
   // 2nd step: filter after minimal length;
   //if(minPWidth > 0) minLengthFilter(peakCandStarts, peakCandEnds, minPWidth);
 qDebug() << 35 ;
   // 3rd step: filter after minimal peak distance:
   //if(minPDist > 0) minDistFilter(peakCandStarts, peakCandEnds, MWs, minPDist);

   // Output for identified peaks

   //list<matD> peakSignals;

   qDebug() << 36 ;

 qDebug() << 4;
   list<double> peakRISums;
   vecD peakRhTops(0);
   vecD meanPeakRITops(0); // needed later for SNR calculation
   //vecD peakEndsRh(0);
   uint peakCount = 0;
   for (auto start = peakCandStarts.begin(), end = peakCandEnds.begin(), listEnd = peakCandStarts.end(); start != listEnd;  ++start, ++end){

      const uint peakLength = *end-*start;

      FFFLog_omp::logText(tr("=================================================================="));
      FFFLog_omp::logText(tr("Peak %1:").arg(peakCount));


      FFFLog_omp::logText(tr("Start (time): %1 End (time): %2").arg(data[0][*start]).arg(data[0][*end]));
      FFFLog_omp::logText(tr("Start (radius): %1 End (radius): %2").arg(data[4][*start]).arg(data[4][*end]));
      parLog << tr("Start of Peak %1 (time);").arg(peakCount) << data[0][*start]  << endl;
      parLog << tr("End of Peak %1 (time);").arg(peakCount) << data[0][*end] << endl;\
      parLog << tr("Start of Peak %1 (radius);").arg(peakCount) << data[4][*start]  << endl;
      parLog << tr("End of Peak %1 (time); ").arg(peakCount) << data[4][*end] << endl;

      FFFLog_omp::logText(tr("Length of peak: %1").arg(peakLength));
      parLog << tr("Length of peak %1;").arg(peakCount) << peakLength << endl;
qDebug() << 45;
      // peak Signal extraction

      matD peakSignals(data.size());
      for(uint i = 0; i < peakSignals.size(); ++i){
         peakSignals[i].resize(peakLength);
         for(uint j = 0; j < peakLength; ++j){
            peakSignals[i][j] = data[i][j + *start];
         }
      }  // write Signals to file

      FFFCsvWriter writer((tr("%1_peak_%2.csv").arg(outFileNameTemplate).arg(peakCount)).toStdString());
      writer.writeFile(peakSignals, headLines);

      const vecD peakDs  = peakSignals[1];
      const vecD peakMws = peakSignals[2];
      const vecD peakRHs = peakSignals[4];
      const vecD peakRIs = peakSignals[6];
      //peakEndsRh.push_back(peakRHs[*end]);

      // calculate dRI sum
      qDebug() << 5;

      FFFLog_omp::logText(tr("----------------------"));

      double RISum(0.0);
      for(uint i=0; i < peakLength; ++i){
         RISum  += peakRIs[i];
      }
      peakRISums.push_back(RISum);
      double RIAverage = RISum / peakLength;
      FFFLog_omp::logText(tr("Sum of diff. RIs:        %1").arg(RISum));
      parLog << "Sum of different RIs;" << RISum << endl;
      FFFLog_omp::logText(tr("Averaged RIs:            %1").arg(RIAverage));
      parLog << "Averaged RIs;" << RIAverage << endl;

      // calculate mean RH according to RI

      double meanRH(0.0);
      for(uint i = 0; i < peakLength; ++i){
         meanRH += peakRHs[i] * peakRIs[i];
      }
      //qDebug() << meanRH;
      meanRH /= RISum;
      FFFLog_omp::logText(tr("Weighted Average Rh:     %1").arg(meanRH));
      parLog << "Weighted Average Rh;" << meanRH << endl;


      // calculate peak massweighted MW
      qDebug() << 6;
      //qDebug() << peakMws[0];
      double weightedMws(0.0);
      for(uint i=0; i < peakLength; ++i){
         weightedMws += peakMws[i] * peakRIs[i];
      }
      weightedMws /= RISum;

      FFFLog_omp::logText(tr("Weighted Average Mw:     %1").arg(weightedMws));
      parLog << "Weighted Average Mw;" << weightedMws << endl;

      //qDebug() << peakMws[0];
      double weightedDs(0.0);
      for(uint i=0; i < peakLength; ++i){
         weightedDs += peakDs[i] * peakRIs[i];
      }
      weightedDs /= RISum;

      FFFLog_omp::logText(tr("Weighted Average D:     %1").arg(weightedDs));
      parLog << "Weighted Average D;" << weightedDs << endl;

      // calculate PeakTop values

      const uint peakTopPos = std::distance(peakRIs.begin(), std::max_element(peakRIs.begin(), peakRIs.end()));
      const uint totTopPeakWidth = 2 * peakTopWidth + 1;



      if(totTopPeakWidth >= peakLength         // make sure that the chosen peak top width is in range of the peak
            || peakTopWidth >= peakTopPos
            || peakTopPos + peakTopWidth >= peakLength
            )
      {
         uint minSize = std::min( peakLength / 2 - 3 , std::min( peakTopPos - peakTopWidth - 1,   peakLength - peakTopPos - 1 ));
         FFFLog_omp::logWarning(tr("Peak Top %1 could not be evaluated with chosen peak Length; choose smaller peak top width than %2.").arg(peakCount).arg(minSize));
      }
      else if (peakTopWidth == 1) {
         FFFLog_omp::logText(tr("D of Top:         %1").arg(peakDs[peakTopPos]));
         parLog << "D of Top;" << peakDs[peakTopPos] << endl;

         FFFLog_omp::logText(tr("Rh of Top:        %1").arg(peakRHs[peakTopPos]));
         parLog << "Rh of Top;" << peakRHs[peakTopPos] << endl;\

         FFFLog_omp::logText(tr("Mw of Top:        %1").arg(peakMws[peakTopPos]));
         parLog << "Mw of Top;" << peakMws[peakTopPos] << endl;
      }
      else {
         uint maxRIPosLimLow = peakTopPos - peakTopWidth;
         uint maxRIPosLimUp  = peakTopPos + peakTopWidth;
         //const uint totTopPeakWidth = 2 * minPTopWidth + 1;

         double meanPeakTopRI(0.0);
         double meanPeakTopRh(0.0);
         double meanPeakTopMW(0.0);
         double meanPeakTopD (0.0);
         for(uint i = maxRIPosLimLow; i < maxRIPosLimUp; ++i){
            meanPeakTopRI += peakRIs[i];
            //meanPeakTopRh += peakRHs[i] * peakRIs[i];
            //meanPeakTopD += peakDs[i] * peakRIs[i];
            meanPeakTopRh += peakRHs[i];
            meanPeakTopD += peakDs[i];
            meanPeakTopMW += peakMws[i];
         }
         peakRhTops.push_back(peakRHs[peakTopPos]);


         meanPeakTopRI /= totTopPeakWidth;
         meanPeakRITops.push_back(meanPeakTopRI);
         meanPeakTopD /= totTopPeakWidth;
         meanPeakTopRh /= totTopPeakWidth;
         meanPeakTopMW /= totTopPeakWidth;
         FFFLog_omp::logText(tr("Average Rh of Top:        %1").arg(meanPeakTopRh));
         parLog << "Average Rh of Top;"  << meanPeakTopRh << endl;
         FFFLog_omp::logText(tr("Average Mw of Top:        %1").arg(meanPeakTopMW));
         parLog << "Average Mw of Top;" << meanPeakTopMW << endl;
         FFFLog_omp::logText(tr("Average D of Top:        %1").arg(meanPeakTopD));
         parLog << "Average D of Top;"   << meanPeakTopD << endl;
      }
      ++peakCount;
   }

   qDebug() << 7;

   double riSum(0.0);
   for(auto ri = peakRISums.begin(); ri != peakRISums.end(); ++ri){
      riSum += *ri;
   }
   //qDebug() << riSum;
   list<double> peakRISumsRel;
   for(auto ri = peakRISums.begin(); ri != peakRISums.end(); ++ri){
      peakRISumsRel.push_back(*ri / riSum);
   }

   {
      FFFLog_omp::logText(tr("=================================================================="));
      FFFLog_omp::logText(tr("RI distribution:"));
      uint i = 0;
      for(auto riRel = peakRISumsRel.begin(); riRel !=  peakRISumsRel.end(); ++riRel){
         FFFLog_omp::logText(tr("Peak %1: %2").arg(i).arg(*riRel));
         parLog << "RI contrib. Peak " << i << "; " << *riRel << endl;
         ++i;
      }
   }
   qDebug() << 8;
   FFFLog_omp::logText(tr("==================================================================\n"));


   {
      FFFLog_omp::logText(tr("=================================================================="));
      FFFLog_omp::logText(tr("Peak Valleys and PTVs:"));



      vector<uint> topPos(peakRhTops.size());
      // find position of i-th top
      for(uint i = 0; i < peakRhTops.size(); ++i){
         topPos[i] = 0;
         while(Rh[topPos[i]] < peakRhTops[i]) ++(topPos[i]);
      }
      qDebug() << 81;

      // find position of i-th valley

      vector<uint> valleyPos(topPos.size() - 1);
      vecD minScope;
      minScope.reserve(dataLength);
      for(uint i = 0; i < valleyPos.size(); ++i){
            qDebug() << i;
         uint minScopeSize = topPos[i+1] - topPos[i] + 1;
         minScope.resize(minScopeSize);
         for(uint j = 0; j <= minScopeSize ; ++j) minScope[j] = RI[j + topPos[i]];
         valleyPos[i] = std::distance(minScope.begin() , std::min_element(minScope.begin(), minScope.end())) + topPos[i];
      }

qDebug() << 9;

      vecD PTVs(valleyPos.size());
      for(uint i = 0; i < valleyPos.size(); ++i){
         PTVs[i] = RI[topPos[i]] / RI[valleyPos[i]];
      }

      for(uint i = 0; i < valleyPos.size(); ++i){
         FFFLog_omp::logText(tr("Rh of valley %1: %2").arg(i).arg(Rh[valleyPos[i]]));
         parLog << "Rh of Peakvalley " << i << "; " << Rh[valleyPos[i]] << endl;
         FFFLog_omp::logText(tr("RI of valley %1: %2").arg(i).arg(RI[valleyPos[i]]));
         parLog << "RI of valley " << i << "; " << RI[valleyPos[i]] << endl;
         FFFLog_omp::logText(tr("PTV %1: %2").arg(i).arg(PTVs[i]));
         parLog << "PTV " << i << "; " << PTVs[i] << endl;
      }

   }

   qDebug() << 10;
   {
      FFFLog_omp::logText(tr("==================================================================\n"));
      FFFLog_omp::logText(tr("Signal-to-Noise ratio:"));

      //qDebug() << peakCandEnds.size() << *peakCandEnds.begin() << peakCandEnds.back();
      uint snScopeStart = peakCandEnds.back() + 600;  // 5 minutes after last peak
      uint snScopeEnd = snScopeStart + 360;          // 3 minutes after scopeStart
      if(snScopeEnd > dataLength){
         FFFLog_omp::logWarning(tr("End of SNR scope not in data range!")); return;
      }
      //qDebug() << 2;
      uint scopeLength = snScopeEnd - snScopeStart;
      //qDebug() << scopeLength;
      vecD snScopeRh = vecD(scopeLength);
      vecD snScopeRI = vecD(scopeLength);

      //copy relevant data
      for(uint i = 0; i < scopeLength; ++i){
         snScopeRh[i] = Rh[i + snScopeStart];
         //qDebug() << snScopeRh[i];
         snScopeRI[i] = RI[i + snScopeStart];
      }

      //----------------------------
      // linear trendline version
      //----------------------------


      // trend line: y = mx + b





      /*
      double m = 0.0;
      double b = 0.0;
      linearRegression(snScopeRh, snScopeRI, &m, &b);
      vecD xTrendLine = snScopeRh;
      vecLD yTrendLine(scopeLength);
     // qDebug() << 4;
      for(uint i = 0; i < scopeLength; ++i)
         yTrendLine[i] = static_cast<lDouble> (m * xTrendLine[i] + b);
      // noise = s_ŷ = sqrt{ (1 / (n-2) \sum( y_i - ŷ_i)² ) }
      lDouble squareSum = 0.0;
      for(uint i = 0; i < scopeLength; ++i)
         squareSum += std::pow( static_cast<lDouble>(snScopeRI[i]) - yTrendLine[i], 2.0L);
      double noise = static_cast<double>(std::sqrt(squareSum / (static_cast<lDouble>(scopeLength) - 2.0L)));

      //qDebug() << 5;
qDebug() << 11;




      FFFLog_omp::logText(tr("SNR scope on RH: %1 to %2").arg(snScopeRh[0]).arg(snScopeRh[scopeLength-1]));
      parLog << "SNR scope start on RH; "  << snScopeRh[0] << endl;
      parLog << "SNR scope end on RH; "    << snScopeRh[scopeLength-1] << endl;
      FFFLog_omp::logText(tr("Regression slope: %1 ").arg(m));
      FFFLog_omp::logText(tr("Regression offset: %1 ").arg(b));
      parLog << "Regression slope; " << *snScopeRh.begin() << endl;
      parLog << "Regression offset; " << *(snScopeRh.end()) << endl;

      vecD SNR(meanPeakRITops);
      for(uint i = 0; i < SNR.size(); ++i){
         SNR[i] /= noise;
         FFFLog_omp::logText(tr("Average RI of Top %1: %2").arg(i).arg(meanPeakRITops[i]));
         parLog << "Average RI of Top " << i << "; " << meanPeakRITops[i] << endl;
         FFFLog_omp::logText(tr("SNR of Peak %1: %2").arg(i).arg(SNR[i]));
         parLog << "SNR " << i << "; " << SNR[i] << endl;
      }

*/
      //----------------------------
      // floating average version
      //----------------------------

      //qDebug() << 2;
      //qDebug() << scopeLength;

      // get average floats;

      const uint avWindowSize = 41;
      vecD snAvRI(scopeLength + avWindowSize + 1);
      qDebug() << snAvRI.size();
      for(uint i = 0; i < scopeLength - avWindowSize + 1; ++i ){
         snAvRI[i] = 0.0;
         for(uint j = 0 ; j < avWindowSize; ++j)
            snAvRI[i] += snScopeRI[i+j];
         snAvRI[i] /= avWindowSize;
      }
      // chop scope ends
      const uint chopSize = (avWindowSize-1)/2;
      snScopeRI.erase(snScopeRI.begin(), snScopeRI.begin() + chopSize);
      snScopeRI.erase(snScopeRI.end() - chopSize, snScopeRI.end());
      snScopeRh.erase(snScopeRh.begin(), snScopeRh.begin() + chopSize);
      snScopeRh.erase(snScopeRh.end() - chopSize, snScopeRh.end());
      scopeLength = snScopeRI.size();
      // noise = s_ŷ = sqrt{ (1 / (n-2) \sum( y_i - ŷ_i)² ) }
      lDouble squareSum = 0.0;
      for(uint i = 0; i < scopeLength; ++i)
         squareSum += std::pow(static_cast<lDouble>(snScopeRI[i] - snAvRI[i]), 2.0L);
      double noise = static_cast<double>(std::sqrt(squareSum / (static_cast<lDouble>(scopeLength) - 2.0L)));

      FFFLog_omp::logText(tr("SNR scope on RH: %1 to %2").arg(snScopeRh[0]).arg(snScopeRh[scopeLength-1]));
      parLog << "SNR scope start on RH; "  << snScopeRh[0] << endl;
      parLog << "SNR scope end on RH; "    << snScopeRh[scopeLength-1] << endl;


      //FFFLog_omp::logText(tr("Regression slope: %1 ").arg(m));
//      FFFLog_omp::logText(tr("Regression offset: %1 ").arg(b));
      //parLog << "Regression slope; " << *snScopeRh.begin() << endl;
      //parLog << "Regression offset; " << *(snScopeRh.end()) << endl;
      FFFLog_omp::logText(tr("noise: %1").arg(noise));
      parLog << "noise " << "; " << noise << endl;

      vecD SNR(meanPeakRITops);
      for(uint i = 0; i < SNR.size(); ++i){
         FFFLog_omp::logText(tr("----------------------------------------------------------------"));
         SNR[i] /= noise;
         FFFLog_omp::logText(tr("Average RI of Top %1: %2").arg(i).arg(meanPeakRITops[i]));
         parLog << "Average RI of Top " << i << "; " << meanPeakRITops[i] << endl;
         FFFLog_omp::logText(tr("SNR of Peak %1: %2").arg(i).arg(SNR[i]));
         parLog << "SNR " << i << "; " << SNR[i] << endl;

         qDebug() << 3;


      }
FFFLog_omp::logText(tr("==================================================================\n\n\n"));
      qDebug() << 12;
   }
}

void mainWindow::minLengthFilter(std::list<uint> &peakCandStarts, std::list<uint> &peakCandEnds, const uint minPWidth)
{
   //qDebug() << "Filter Length" << peakCandStarts.size() << peakCandEnds.size() << minPWidth;

   for (auto start = peakCandStarts.begin(), end = peakCandEnds.begin(), listEnd = peakCandStarts.end(); start != listEnd;  ++start, ++end)
   {
      //qDebug() << "Test" << *start << *end;
      if(*end - *start < minPWidth){
         FFFLog_omp::logText(tr("Candidate from %1 to %2 too short, the data island is skipped.").arg(*start).arg(*end));
         peakCandStarts.erase(start);
         peakCandEnds.erase(end);
      }
   }
}

void mainWindow::minDistFilter(std::list<uint> &peakCandStarts, std::list<uint> &peakCandEnds, const vecD &signal, const uint minPDist)
{

   for (auto start = peakCandStarts.begin(), end = peakCandEnds.begin(), listEnd = peakCandStarts.end(); start != listEnd;  ++start, ++end)
   {
      const uint startPos = *start;
      if (startPos < minPDist) continue;
      const uint endPos = *end;
      if (endPos < minPDist) continue;

      bool foundWrongDist = false;

      // check before start
      for(uint i = startPos - minPDist; i < startPos; ++i){
         if(!isZero(signal[i])){
            foundWrongDist = true; break;
         }
      }
      // check after end
      if(!foundWrongDist){
         for(uint i = endPos+1; i < endPos + minPDist; ++i){
            if(!isZero(signal[i])){
               foundWrongDist = true; break;
            }
         }
      }
      // remove scope if necessary
      if(foundWrongDist){
         FFFLog_omp::logText(tr("Zero Island around the scope from %1 to %2 too short, the data scope is skipped.").arg(*start).arg(*end));
         peakCandStarts.erase(start);
         peakCandEnds.erase(end);
      }
   }
}

int mainWindow::linearRegression(const vecD &xValues,
                                 const vecD &yValues,
                                 double *slope,
                                 double *offset) const
{
   if((xValues.size() != yValues.size())
         || (xValues.size() != yValues.size())
         ){
      return 1;
   }

   double sumX(0.0);
   double sumY(0.0);
   double sumXY(0.0);
   double sumXSq(0.0);
   uint n = xValues.size();
   for(uint i = 0; i < xValues.size(); ++i){
      sumX += xValues.at(i);
      sumY += yValues.at(i);
      sumXY += xValues.at(i) * yValues.at(i);
      sumXSq += xValues.at(i) * xValues.at(i);
   }
   *slope = (n * sumXY - sumX * sumY) / (n * sumXSq - sumX * sumX);
   *offset = (sumY - ((*slope) * sumX)) / n;
   return 0;
}




















