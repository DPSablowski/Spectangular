#ifndef BARYCORRDIALOG_H
#define BARYCORRDIALOG_H

// Dialog Tools -> Barycentric correction: computes the barycentric correction of every
// spectrum (module barycorr) and writes the spectra shifted to the barycentre, optionally
// also the start RVs, or only the table of the corrections (BC file of the Data tab, for the
// model: spectra in the observatory frame or already corrected ones).

#include <QDialog>
#include <QString>
#include <QStringList>

#include <functional>
#include <string>
#include <vector>

class QLineEdit;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;
class QLabel;
class QPushButton;
class QTableWidget;

class BaryCorrDialog : public QDialog
{
    Q_OBJECT

public:
    // reads spectrum i (index as in the file names): wavelengths, fluxes
    using Reader = std::function<bool(int index, std::vector<double> &w, std::vector<double> &f, std::string &err)>;

    struct MainData {
        QString work, prefix, ext;      // spectra <prefix><i><ext> in the work folder
        int first = 0, last = 0;
        bool fits = false;              // input as FITS (output always ASCII)
        QString timesFile;              // times of observation, one line per spectrum
        QString rvFile;                 // start RVs (optional): one line per spectrum
        int rvColumns = 2;              // SB2: RV_A RV_B, SB1: RV_A
        // further files with one line per spectrum (S/N, BC, strengths, ...), adapted when spectra
        // are rejected (with the times and RV file and edit.dat)
        QStringList perSpectrum;
        Reader read;
    };

    explicit BaryCorrDialog(QWidget *parent = nullptr);
    void setMainData(const MainData &d);
    const std::vector<double> &corrections() const { return bcs; }

signals:
    // new prefix, extension, RV file ("" if not written)
    void useInMain(const QString &prefix, const QString &ext, const QString &rvFile);
    // BC file written (name in the work folder); corrected: the spectra are already corrected
    void useBcFile(const QString &file, bool corrected);
    // airmass file written (one value per spectrum): start strengths of the static lines
    void useAirmassFile(const QString &file);
    // spectra rejected (renamed, the others renumbered): new last index; note for the messages
    void spectraRejected(int last, const QString &note);

public slots:
    void on_pushButton_compute_clicked();
    void on_pushButton_write_clicked();
    void on_pushButton_use_clicked();
    void on_pushButton_writebc_clicked();
    void on_pushButton_tellcheck_clicked();
    void on_pushButton_reject_clicked();

private:
    bool compute(QString &err);
    bool writeTable(const QString &file, QString &err) const;
    bool writeAirmass(const QString &file, QString &err) const;
    void siteChanged(int i);
    void markRejects();
    bool rejectSpectra(const std::vector<int> &rows, QString &report, QString &err);

    MainData md;
    std::vector<double> times, jdUtc, bcs, airmasses;
    std::vector<double> tellShift, tellDev, tellCorr;      // check of the telluric lines
    QString writtenPrefix, writtenExt, writtenRv;

    QLabel *labelSpectra = nullptr, *labelResult = nullptr;
    QLineEdit *eRa = nullptr, *eDec = nullptr, *eOut = nullptr, *eBc = nullptr, *eAm = nullptr;
    QComboBox *site = nullptr, *scale = nullptr;
    QDoubleSpinBox *lat = nullptr, *lon = nullptr, *height = nullptr;
    QLineEdit *eTimes = nullptr;
    QCheckBox *cRv = nullptr, *cCorrected = nullptr;
    QPushButton *bCompute = nullptr, *bWrite = nullptr, *bUse = nullptr, *bWriteBc = nullptr;
    QPushButton *bTellCheck = nullptr, *bReject = nullptr;
    QDoubleSpinBox *tellLimit = nullptr;
    QTableWidget *table = nullptr;
};

#endif // BARYCORRDIALOG_H
