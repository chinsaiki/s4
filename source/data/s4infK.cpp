
#include "data/s4infK.h"
#include "common/s4exceptions.h"

namespace S4 {

void infKQ_t::add_vec_dayK(vec_dayK_t& KQ) {
	// if (_tMode == tMINU) {
	// 	throw InfoError("Cannot add dayK to minuK!");
	// }
	assert(_tMode == tUNKNOWN || _tMode == tDAY);
	assert(isNewAtBack());

	this->reserve(KQ.size());
	for (vec_dayK_t::const_iterator i = KQ.begin(); i != KQ.end(); ++i) {
		assert(size()==0 || i->date > back()->_date);
		emplace_back(std::make_shared<infK_t>(*i));
	}
	_tMode = tDAY;
}

void infKQ_t::add_vec_minuK(vec_minuK_t& KQ) {
	// if (_tMode == tDAY) {
	// 	throw InfoError("Cannot add minuK to dayK!");
	// }
	assert(_tMode == tUNKNOWN || _tMode == tMINU);
	assert(isNewAtBack());

	this->reserve(KQ.size());
	for (vec_minuK_t::const_iterator i = KQ.begin(); i != KQ.end(); ++i) {
		assert(size() == 0 || i->time > back()->_time);
		emplace_back(std::make_shared<infK_t>(*i));
	}
	_tMode = tMINU;
}

void infKQ_t::calcFQ(const vec_gbbd_t* gbbd, const vec_fhps_t* fhps)
{
	bool needSwap = !isNewAtBack();
	if (needSwap) {
		newAtBack();
	}

	for (infKQ_t::const_iterator i = begin(); i != end(); ++i) {
		//	push_back(new infK_t(*i));
		(*i)->_fh_baseTime = front()->_time + back()->_time ;
	}
	calcGB(gbbd);
	calcFH(fhps);
	if (needSwap) {
		newAtFront();
	}
}

void infKQ_t::calcFH(const vec_fhps_t* fhps) {
	if (fhps == NULL)
		return;
	double fct_fh, fct_sg, fct_pg;
	size_t apply_day = 0, k;
	for (vec_fhps_t::const_iterator f = fhps->begin(); f != fhps->end(); ++f) {
		while (apply_day < size() && (uint32_t)(*this)[apply_day]->_date < f->strDate) {
			apply_day++;
		}
		if (apply_day == 0)	//first day is fh day, no factor to apply
			continue;

		fct_fh = 1.0 - f->hongli * 100 / (*this)[apply_day - 1]->close;
		fct_sg = 1 / (1 + f->songgu);
		//search apply day for peigu
		fct_pg = 1.0;
		if (f->peigujia > 0 && f->peigu > 0) {
			for (k = apply_day; k < size(); ++k) {
				if ((*this)[k]->_zong*1.01 > (*this)[apply_day - 1]->_zong*(1.0 + (double)f->peigu)) {
					break;
				}
			}
			if (k != size()) {
				double pg_apply_nb = (*this)[k]->_zong - (*this)[apply_day - 1]->_zong;
				fct_pg = ((*this)[apply_day - 1]->_zong + pg_apply_nb * f->peigujia*100 / (*this)[apply_day - 1]->close)
					/
					(*this)[k]->_zong;
			}
		}

		for (size_t i = 0; i != apply_day; ++i) {
			(*this)[i]->_factor *= fct_fh * fct_sg * fct_pg;
		}
		if (apply_day >= size())
			break;
	}
}

void infKQ_t::calcGB(const vec_gbbd_t* gbbd) {
	if (gbbd == NULL)
		return;
	//set guben
	vec_gbbd_t::const_iterator g = gbbd->begin();
	float hoult, houz;
	//if (g->qianliutong != 0 || g->qianzong != 0) {
	//	printf("[W] first gbbd.qianliutong!=0 || qianzong!=0:(%d  %f  %f)\n", g->strDate, g->qianliutong, g->qianzong);
	//}
	hoult = g->houliutong;
	houz = g->houzong;
	g++;
	size_t i = 0;
	for (; g != gbbd->end() && i < size(); ++g) {
		if ((uint32_t)(*this)[i]->_date >= g->strDate) {
			hoult = g->houliutong;
			houz = g->houzong;
			continue;
		}
		for (; i < size(); ++i) {
			if ((uint32_t)(*this)[i]->_date >= g->strDate) {
				hoult = g->houliutong;
				houz = g->houzong;
				break;
			}
			(*this)[i]->_liutong = hoult;
			(*this)[i]->_zong = houz;
            (*this)[i]->_turnOver = (float)(*this)[i]->volume / hoult;
		}
	}
	for (; i < size(); ++i) {
		(*this)[i]->_liutong = hoult;
		(*this)[i]->_zong = houz;
        (*this)[i]->_turnOver = (float)(*this)[i]->volume / hoult;
	}
}

}
