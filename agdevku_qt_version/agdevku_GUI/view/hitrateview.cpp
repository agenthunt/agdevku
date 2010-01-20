#include "hitrateview.h"


HitRateView::HitRateView(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	bufMgr = BufferManager::getInstance();

	float hitRate = bufMgr->getHitRate();
	hitRateLabel = new QLabel(tr("NUM OF TIMES PAGE IS IN FOUND IN BUFFER  /  NO OF PAGE REQUESTS TO BUFFER"));
	hitRateText = new QLineEdit();
	hitRateText->setReadOnly(true);
	hitRateText->setText(QString::number(hitRate));
	mainLayout->setAlignment(Qt::AlignTop);
	mainLayout->addWidget(hitRateLabel);
	mainLayout->addWidget(hitRateText);
}

HitRateView::~HitRateView()
{

}

void HitRateView::refreshView(){
	float hitRate = bufMgr->getHitRate();
	hitRateText->setText(QString::number(hitRate));
}
