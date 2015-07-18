import FWCore.ParameterSet.Config as cms

def customise_localreco(process):
    print "# customising to local reco"
    process.reconstruction_step.replace(process.MeasurementTrackerEvent,(process.digifilter_step*process.MeasurementTrackerEvent))
    
    ### removing DQM modules that complain
    process.pathALCARECOTkAlCosmicsCTF0T.remove(process.ALCARECOTkAlCosmicsCTF0TDQM)
    process.pathALCARECOTkAlCosmicsRegional0T.remove(process.ALCARECOTkAlCosmicsRegional0TDQM)
    process.pathALCARECOTkAlCosmicsCosmicTF0T.remove(process.ALCARECOTkAlCosmicsCosmicTF0TDQM)
    process.pathALCARECOTkAlCosmicsCTF0THLT.remove(process.ALCARECOTkAlCosmicsCTF0TDQM)
    process.pathALCARECOTkAlCosmicsRegional0THLT.remove(process.ALCARECOTkAlCosmicsRegional0TDQM)
    process.pathALCARECOTkAlCosmicsCosmicTF0THLT.remove(process.ALCARECOTkAlCosmicsCosmicTF0TDQM)

    return process
