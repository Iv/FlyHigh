#ifndef _SwissGrid_h_
#define _SwissGrid_h_

void LLtoSwissGrid(const double Lat, const double Long,
			 double &SwissNorthing, double &SwissEasting);
void SwissGridtoLL(const double SwissNorthing, const double SwissEasting, 
					double& Lat, double& Long);

#endif
