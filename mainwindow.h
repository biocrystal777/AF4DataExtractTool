#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGuiApplication>
#include <QScreen>
#include <QWidget>
#include <QGridLayout>
#include <QSettings>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QGroupBox>
#include <QRadioButton>
#include <QFile>
#include <utility>
#include <cmath>

#include "./ffffileinoutwidget.h"
#include "./ffflog_omp.h"
#include "./fffcsvparser.h"
#include "./fffcsvwriter.h"

class mainWindow : public QWidget
{
    Q_OBJECT

public:
   explicit mainWindow(QWidget *parent = 0);
   ~mainWindow(){
      saveParameters();
   }


private:
    QGridLayout *thisLay = nullptr;

    FFFFileInOutWidget *fileWidget = nullptr;
    // log Widget
    FFFLog_omp *logWidget = nullptr;


    QGroupBox    *sepMethodBox        = nullptr;
    QGridLayout  *sepMethodBoxLay     = nullptr;

    QRadioButton *derivativeSepBut    = nullptr;
    QFrame       *derivativeSepFrame  = nullptr;
    QGridLayout *derivativeSepLay     = nullptr;

    QRadioButton *thresholdSepBut     = nullptr;
    QFrame       *thresholdSepFrame   = nullptr;
    QGridLayout *thresholdSepLay      = nullptr;
    QCheckBox *useThreshold = nullptr;
    QDoubleSpinBox *peakThreshold = nullptr;
    QSpinBox *minPeakWidth        = nullptr;
    QSpinBox *minPeakDistance     = nullptr;
    QSpinBox *minPeakTopWidth     = nullptr;
    QPushButton *start            = nullptr;

    void loadParameters();
    void saveParameters();

    inline bool isZero(double x) const { return ( x <= 0.0) && (x >= 0.0); }

    void minLengthFilter(std::list<uint> &peakCandStarts, std::list<uint> &peakCandEnds, const uint minPWidth);
    void minDistFilter(std::list<uint> &peakCandStarts, std::list<uint> &peakCandEnds, const vecD &signal, const uint minPDist);

    /*!
     * \brief linearRegression calculates a linear regression for a given data set Y(X)
     * \param xValues values of the x-axis
     * \param yValues values of the y-axis
     * \param pointRelevant indicates which point of x and y shall be considered for the regression
     * \param slope of the regression line
     * \param offset pffest of the regression line
     * \return error Code
     */
    int linearRegression(const vecD &xValues,
                         const vecD &yValues,
                         double *slope,
                         double *offset) const;

private slots:
    void startExtraction();






};

#endif // MAINWINDOW_H
