#include "SamPerformanceControls.h"

SamPerformanceControls::SamPerformanceControls(AModel *parent)
	: AModel(parent)
{
	m_envelopeAttack       = 1.0f;
	m_envelopeAttackOffset = 1.0f;
	m_envelopeHold         = 1.0f;
	m_envelopeSustain      = 1.0f;
	m_envelopeDecay        = 1.0f;
	m_envelopeRelease      = 1.0f;

	m_envelopeReleaseAttack       = 1.0f;
	//m_envelopeReleaseAttackOffset = 1.0f;
	m_envelopeReleaseHold         = 1.0f;
	m_envelopeReleaseSustain      = 1.0f;
	m_envelopeReleaseDecay        = 1.0f;
	m_envelopeReleaseRelease      = 1.0f;
	
	m_sampleStart = 1.0f;
	m_cellSampleStart = 1.0f;

	m_minimumVelocity = 0;

	m_filterCutOff = 127;
	m_fFilterCutOff = 1.0f;
}

SamPerformanceControls::~SamPerformanceControls()
{

}

void SamPerformanceControls::setMinimumVelocity(float value)
{
	value *= 127.0;
	m_minimumVelocity = (int)value;
}

void SamPerformanceControls::setFilterCutOff(float value)
{
	m_fFilterCutOff = value;
	value *= 127.0;
	m_filterCutOff = (int)value;
}

void SamPerformanceControls::debug(void)
{
	aLog("---------- SamPerformanceControls::debug() ----------");
	aLog("m_envelopeAttack = %f", m_envelopeAttack);
	aLog("m_envelopeAttackOffset = %f", m_envelopeAttackOffset);
	aLog("m_envelopeHold = %f", m_envelopeHold);
	aLog("m_envelopeSustain = %f", m_envelopeSustain);
	aLog("m_envelopeDecay = %f", m_envelopeDecay);
	aLog("m_envelopeRelease = %f", m_envelopeRelease);

	aLog("m_envelopeReleaseAttack = %f", m_envelopeReleaseAttack);
	//aLog("m_envelopeReleaseAttackOffset = %f", m_envelopeReleaseAttackOffset);
	aLog("m_envelopeReleaseHold = %f", m_envelopeReleaseHold);
	aLog("m_envelopeReleaseSustain = %f", m_envelopeReleaseSustain);
	aLog("m_envelopeReleaseDecay = %f", m_envelopeReleaseDecay);
	aLog("m_envelopeReleaseRelease = %f", m_envelopeReleaseRelease);

	aLog("m_sampleStart = %f", m_sampleStart);
	aLog("m_cellSampleStart = %f", m_cellSampleStart);

	aLog("m_minimumVelocity = %d", m_minimumVelocity);

	aLog("m_filterCutOff = %d", m_filterCutOff);
	aLog("m_fFilterCutOff = %f", m_fFilterCutOff);
	aLog("-----------------------------------------------------");
}