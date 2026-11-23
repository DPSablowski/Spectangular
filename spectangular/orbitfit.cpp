#include "orbitfit.h"
#include "ui_orbitfit.h"

#include "dataio.h"
#include "qcustomplot.h"

#include <QFile>
#include <QMessageBox>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace {
const char *kNames[8] = {"P", "e", "K_A", "K_B", "gamma", "T_peri", "omega_A", "offset_B"};
}

OrbitFit::OrbitFit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrbitFit)
{
    ui->setupUi(this);
    setWindowTitle("Orbit fit");
    ui->checkBox_2->setChecked(true);
    ui->checkBox_file->setChecked(true);
    ui->lineEdit->setText("rvs.dat");
    ui->checkBox_main->setEnabled(false);
    connect(ui->pushButton, &QPushButton::clicked, this, &QDialog::close);
    for(QCustomPlot *p : {ui->customPlot, ui->customPlot_res}){
        p->xAxis2->setVisible(true);
        p->xAxis2->setTickLabels(false);
        p->yAxis2->setVisible(true);
        p->yAxis2->setTickLabels(false);
        p->xAxis->setLabel("Orbital Phase");
        p->xAxis->setRange(0.0, 1.0);
    }
    ui->customPlot->yAxis->setLabel("RV [km/s]");
    ui->customPlot_res->yAxis->setLabel("O-C [km/s]");
    ui->pushButton_write->setEnabled(false);
    // room for three lines of statistics (a word-wrapped label in a grid gets too little height)
    ui->labelStats->setMinimumHeight(3*ui->labelStats->fontMetrics().lineSpacing() + 4);
    ui->labelStats->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

OrbitFit::~OrbitFit()
{
    delete ui;
}

void OrbitFit::setMainData(const QString &workPath, const std::vector<double> &t, const std::vector<double> &rvA,
                           const std::vector<double> &rvB, bool sb2, bool valid, const QString &note,
                           bool offsetB)
{
    ui->checkBox_off->setChecked(offsetB);
    ui->lineEdit_2->setText(workPath);
    mainT = t;
    mainA = rvA;
    mainB = rvB;
    mainSb2 = sb2;
    mainValid = valid && !t.empty() && t.size() == rvA.size() && (!sb2 || rvB.size() == t.size());
    ui->checkBox_main->setEnabled(mainValid);
    ui->checkBox_main->setToolTip(note);
    if(mainValid){
        ui->checkBox_main->setChecked(true);
        ui->checkBox_file->setChecked(false);
        ui->checkBox->setChecked(!sb2);
        ui->checkBox_2->setChecked(sb2);
    }
    ui->labelData->setText(note);
    if(QFile::exists(workPath+"/"+orbitFile))
        on_pushButton_load_clicked();
}

void OrbitFit::on_checkBox_clicked() { ui->checkBox->setChecked(true); ui->checkBox_2->setChecked(false); }
void OrbitFit::on_checkBox_2_clicked() { ui->checkBox_2->setChecked(true); ui->checkBox->setChecked(false); }
void OrbitFit::on_checkBox_main_clicked() { ui->checkBox_main->setChecked(true); ui->checkBox_file->setChecked(false); }
void OrbitFit::on_checkBox_file_clicked() { ui->checkBox_file->setChecked(true); ui->checkBox_main->setChecked(false); }

void OrbitFit::on_pushButton_load_clicked()
{
    std::vector<double> el;
    std::string err;
    if(!dataio::readNumbers((ui->lineEdit_2->text()+"/"+orbitFile).toStdString(), el, err) || el.size()<7){
        QMessageBox::information(this, "Orbit fit", orbitFile+" not found or incomplete in the work folder.");
        return;
    }
    QLineEdit *f[7] = {ui->lineEdit_17, ui->lineEdit_18, ui->lineEdit_19, ui->lineEdit_20,
                       ui->lineEdit_21, ui->lineEdit_22, ui->lineEdit_23};
    for(int i=0; i<7; i++)
        f[i]->setText(QString::number(i==6 ? el[i]*180.0/M_PI : el[i], 'g', 12));
}

bool OrbitFit::collectData(rvfit::Data &d, QString &err)
{
    const bool sb2 = ui->checkBox_2->isChecked();
    if(ui->checkBox_main->isChecked()){
        if(!mainValid){ err = "No times and RVs from the main window."; return false; }
        d = rvfit::Data();
        d.t = mainT;
        d.rvA = mainA;
        if(sb2) d.rvB = mainB;
        return true;
    }
    std::string e;
    if(!rvfit::readTable((ui->lineEdit_2->text()+"/"+ui->lineEdit->text()).toStdString(), sb2, d, e)){
        err = QString::fromStdString(e);
        return false;
    }
    return true;
}

void OrbitFit::on_pushButton_2_clicked()
{
    rvfit::Data d;
    QString err;
    if(!collectData(d, err)){
        QMessageBox::information(this, "Error", "Orbit fit: "+err);
        return;
    }
    d.offsetB = d.sb2() && ui->checkBox_off->isChecked();
    QLineEdit *f[7] = {ui->lineEdit_17, ui->lineEdit_18, ui->lineEdit_19, ui->lineEdit_20,
                       ui->lineEdit_21, ui->lineEdit_22, ui->lineEdit_23};
    QCheckBox *fx[7] = {ui->checkBox_3, ui->checkBox_4, ui->checkBox_5, ui->checkBox_6,
                        ui->checkBox_7, ui->checkBox_8, ui->checkBox_9};
    double start[7];
    bool fixed[7];
    for(int i=0; i<7; i++){
        bool ok = false;
        start[i] = f[i]->text().toDouble(&ok);
        if(!ok || !std::isfinite(start[i])){
            QMessageBox::information(this, "Error", QString("Orbit fit: start value of %1 missing or not a number.").arg(kNames[i]));
            return;
        }
        fixed[i] = fx[i]->isChecked();
    }
    if(!(start[0]>0) || !(start[1]>=0 && start[1]<1) || start[2]<0 || start[3]<0){
        QMessageBox::information(this, "Error", "Orbit fit: start values need P > 0, 0 <= e < 1 and K >= 0.");
        return;
    }
    start[6] *= M_PI/180.0;
    setCursor(Qt::WaitCursor);
    res = rvfit::fit(d, start, fixed, ui->spinBox->value());
    setCursor(Qt::ArrowCursor);
    if(!res.ok){
        QMessageBox::information(this, "Error", "Orbit fit: "+(res.message.empty()
                                 ? QString("no valid solution; check the start values and the data.")
                                 : QString::fromStdString(res.message)));
        return;
    }
    QLineEdit *v[7] = {ui->lineEdit_3, ui->lineEdit_4, ui->lineEdit_5, ui->lineEdit_6, ui->lineEdit_7,
                       ui->lineEdit_8, ui->lineEdit_9};
    QLineEdit *e[7] = {ui->lineEdit_10, ui->lineEdit_11, ui->lineEdit_12, ui->lineEdit_13, ui->lineEdit_14,
                       ui->lineEdit_15, ui->lineEdit_16};
    for(int i=0; i<7; i++){
        const double s = i==6 ? 180.0/M_PI : 1.0;
        const bool used = std::find(res.freeIdx.begin(), res.freeIdx.end(), i)!=res.freeIdx.end();
        v[i]->setText(QString::number(res.el[i]*s, 'g', 12));
        e[i]->setText(used ? QString::number(res.err[i]*s, 'g', 3) : (fixed[i] ? "fixed" : "-"));
    }
    QString stats = QString("RMS A %1, B %2 km/s; chi² %3 for %4 degrees of freedom; %5 (%6 steps)")
                    .arg(res.rmsA, 0, 'g', 3).arg(d.sb2() ? QString::number(res.rmsB, 'g', 3) : "-")
                    .arg(res.chi2, 0, 'g', 4).arg(res.dof).arg(QString::fromStdString(res.message))
                    .arg(res.iterations);
    if(d.offsetB) stats += QString("\nOffset of B: %1 ± %2 km/s").arg(res.offB, 0, 'f', 3).arg(res.errOffB, 0, 'g', 3);
    ui->labelStats->setText(stats);
    ui->pushButton_write->setEnabled(true);
    plotResult(d);
    writeResultFile(d);
}

void OrbitFit::plotResult(const rvfit::Data &d)
{
    QCustomPlot *p = ui->customPlot, *q = ui->customPlot_res;
    p->clearGraphs();
    q->clearGraphs();
    QVector<double> mx, ma, mb;
    for(int k=0; k<=400; k++){
        const double ph = k/400.0;
        double vA, vB;
        rvfit::model(res.el, res.el[5] + ph*res.el[0], vA, vB);
        mx << ph; ma << vA; mb << vB + res.offB;
    }
    QVector<double> px, pa, pb, ra, rb;
    for(size_t j=0; j<d.t.size(); j++){
        px << rvfit::phase(d.t[j], res.el[0], res.el[5]);
        pa << d.rvA[j];
        ra << res.resA[j];
        if(d.sb2()){ pb << d.rvB[j]; rb << res.resB[j]; }
    }
    auto curve = [&](QCustomPlot *pl, const QVector<double> &x, const QVector<double> &y, QColor c, bool points){
        pl->addGraph();
        QCPGraph *g = pl->graph(pl->graphCount()-1);
        g->setData(x, y);
        g->setPen(QPen(c));
        if(points){
            g->setLineStyle(QCPGraph::lsNone);
            g->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, c, 6));
        }
    };
    const QColor cA(31,119,180), cB(214,39,40);
    curve(p, mx, ma, cA, false);
    curve(p, px, pa, cA, true);
    if(d.sb2()){
        curve(p, mx, mb, cB, false);
        curve(p, px, pb, cB, true);
    }
    p->rescaleAxes();
    p->yAxis->scaleRange(1.12, p->yAxis->range().center());   // room for the points at the extremes
    p->xAxis->setRange(0.0, 1.0);
    p->replot();
    curve(q, QVector<double>{0.0, 1.0}, QVector<double>{0.0, 0.0}, Qt::gray, false);
    curve(q, px, ra, cA, true);
    if(d.sb2()) curve(q, px, rb, cB, true);
    q->rescaleAxes();
    q->yAxis->scaleRange(1.2, q->yAxis->range().center());
    q->xAxis->setRange(0.0, 1.0);
    q->replot();
}

void OrbitFit::writeResultFile(const rvfit::Data &d)
{
    std::ofstream o((ui->lineEdit_2->text()+"/orbitfit_result.txt").toStdString().c_str());
    o<<std::setprecision(12);
    o<<"# Orbit fit to "<<d.t.size()<<" epochs ("<<(d.sb2() ? "SB2" : "SB1")<<"), "<<res.message<<"\n";
    o<<"# RMS A "<<res.rmsA<<" km/s";
    if(d.sb2()) o<<", RMS B "<<res.rmsB<<" km/s";
    o<<"; chi2 "<<res.chi2<<", dof "<<res.dof<<"; errors scaled with chi2/dof\n";
    if(d.offsetB) o<<"# offset of the RVs of B: "<<res.offB<<" +- "<<res.errOffB<<" km/s (model B + offset)\n";
    o<<"# element\tvalue\terror   (omega_A in rad; T_peri: passage nearest to the mean time)\n";
    for(int i=0; i<7; i++){
        const bool used = std::find(res.freeIdx.begin(), res.freeIdx.end(), i)!=res.freeIdx.end();
        o<<kNames[i]<<"\t"<<res.el[i]<<"\t";
        if(used) o<<res.err[i];
        else o<<"fixed";
        o<<"\n";
    }
    o<<"\n# covariance of the free elements:";
    for(int k : res.freeIdx) o<<" "<<kNames[k];
    o<<"\n"<<res.cov;
    o<<"\n# time\tphase\tRV_A\tO-C_A"<<(d.sb2() ? "\tRV_B\tO-C_B" : "")<<"\n";
    for(size_t j=0; j<d.t.size(); j++){
        o<<d.t[j]<<"\t"<<rvfit::phase(d.t[j], res.el[0], res.el[5])<<"\t"<<d.rvA[j]<<"\t"<<res.resA[j];
        if(d.sb2()) o<<"\t"<<d.rvB[j]<<"\t"<<res.resB[j];
        o<<"\n";
    }
}

void OrbitFit::on_pushButton_write_clicked()
{
    if(!res.ok) return;
    std::ofstream o((ui->lineEdit_2->text()+"/"+orbitFile).toStdString().c_str());
    o<<std::setprecision(14);
    for(int i=0; i<7; i++) o<<res.el[i]<<(i<6 ? "\n" : "");
}
