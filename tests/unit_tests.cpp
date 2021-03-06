#include <QtTest/QtTest>
#include <QObject>

#include "GestureAlgos.h"

class TestGestureAlgos: public QObject
{
    Q_OBJECT
private slots:
    void imageToScreenFilterKalman();
	void filterLowPass();
	void isTouch();
private:
	int getData(QVector<int> &x, QVector<int> &y, QVector<qreal> &depth, 
		const QString &fileName);
};

void TestGestureAlgos::imageToScreenFilterKalman()
{
	GestureAlgos *algos = GestureAlgos::instance();
	QVERIFY(NULL != algos);
	QPointF pt(0, 0);
	QVERIFY(EXIT_FAILURE == algos->filterKalman(pt, pt));
	QVERIFY(EXIT_FAILURE == algos->imageToScreen(pt));
	algos->setScreenSize(QSize(1024, 768));
	QVERIFY(EXIT_SUCCESS == algos->filterKalman(pt, pt));
	QVERIFY(EXIT_FAILURE == algos->imageToScreen(pt));
	algos->setImageSize(QSize(320, 240));
	QVERIFY(EXIT_SUCCESS == algos->filterKalman(pt, pt));
	QVERIFY(EXIT_FAILURE == algos->imageToScreen(pt));
	algos->setCorrectionFactors(1.0, QPoint(0, 0));
	QVERIFY(EXIT_SUCCESS == algos->filterKalman(pt, pt));
	pt.setX(0);
	pt.setY(0);
	QVERIFY(EXIT_SUCCESS == algos->imageToScreen(pt));
	QVERIFY(1024.0 == pt.x());
	QVERIFY(0.0 == pt.y());
}

void TestGestureAlgos::filterLowPass()
{
	GestureAlgos *algos = GestureAlgos::instance();
	QVERIFY(NULL != algos);

	qreal fn = 0.1;
	const static qreal PI = 3.1415926535;
	const static qreal TOL = 1e-3;
	int nbSamp = static_cast<int>(10/fn);
	qreal val[] = {0,0};
	qreal sum[] = {0,0};
	for (int n = 0; n < nbSamp; ++n) {
		val[0] = val[1] = sin(2*PI*fn*n);
		algos->filterLowPass(val[0], val[1]);
		sum[0] += abs(val[0]*val[0]);
		sum[1] += abs(val[1]*val[1]);
	}
	QVERIFY(fabs(48.5314 - sum[0]) < TOL);
	QVERIFY(fabs(48.5314 - sum[1]) < TOL);

	fn = 0.4;
	nbSamp = static_cast<int>(10/fn);
	sum[0] = sum[1] = 0;
	for (int n = 0; n < nbSamp; ++n) {
		val[0] = val[1] = sin(2*PI*fn*n);
		algos->filterLowPass(val[0], val[1]);
		sum[0] += abs(val[0]*val[0]);
		sum[1] += abs(val[1]*val[1]);
	}
	QVERIFY(fabs(1.58709 - sum[0]) < TOL);
	QVERIFY(fabs(1.58709 - sum[1]) < TOL);
}

int TestGestureAlgos::getData(QVector<int> &x, QVector<int> &y, QVector<qreal> &depth, 
		const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return EXIT_FAILURE;
	}
	QTextStream istream(&file);
	
	while (!istream.atEnd()) {
		QString line = istream.readLine();
		int idx = line.indexOf(") = (");
		if (0 <= idx) {
			QRegExp rx("(\\d+) , (\\d+) , (\\d*\\.\\d+)");
			int pos = rx.indexIn(line, idx);
			if (-1 < pos) {
				x.push_back(rx.cap(1).toInt());
				y.push_back(rx.cap(2).toInt());
				depth.push_back(rx.cap(3).toFloat());
			}
		}
	}
	
	file.close();
	return EXIT_SUCCESS;
}

void TestGestureAlgos::isTouch()
{
	/*QVector<int> x;
	QVector<int> y;
	QVector<qreal> depth;
	QVERIFY(EXIT_SUCCESS == getData(x, y, depth, "../../tests/ULTRABOOK-BC_vts8_tap.LOG"));

	GestureAlgos *algos = GestureAlgos::instance();
	QVERIFY(NULL != algos);

	//start to process from index 800 in order to detect 3 taps
	int count = 0;
	for (int i = 800; i < x.size(); ++i) {		
		algos->filterLowPass(depth[i]);//apply low pass filter to depth signal
		if (algos->isTouch(depth[i])) {
			if (850 < i) ++count; //skip transition in the filtered signal
		}
	}
	QVERIFY(3 == count);*/
}

QTEST_MAIN(TestGestureAlgos)
#include "unit_tests.moc"