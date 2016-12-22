import FWCore.ParameterSet.Config as cms

SiStripGainAverage = cms.EDAnalyzer('SiStripConditionsReader',
                                    rawFileName = cms.untracked.string("")
                                    )

