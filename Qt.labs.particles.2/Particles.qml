import QtQuick 2.0
import QtQuick.Particles 2.0

ParticleSystem {
    id: container
    property alias source: painter.source
    property alias count: emitter.maximumEmitted
    property alias emissionRate: emitter.emitRate
    property alias lifeSpan: emitter.lifeSpan
    property alias lifeSpanDeviation: emitter.lifeSpanVariation

    //NOTE: fadeInDuration is not fully implemented to behave the same
    property int fadeInDuration: 200 //Not actually as easy to do in Particles 2.0 :|
    onFadeInDurationChanged: setEntryEffect();
    property int fadeOutDuration: 300
    onFadeOutDurationChanged: setEntryEffect();
    function setEntryEffect() //Primary usecase of setting duration was just to turn it off
    {
        if (fadeInDuration > 0 && fadeOutDuration > 0) {
            painter.entryEffect = ImageParticle.Fade;
        } else {
            painter.entryEffect = ImageParticle.None;
        }
    }

    property alias angle: dir.angle;
    property alias angleDeviation: dir.angleVariation
    property alias velocity: dir.magnitude
    property alias velocityDeviation: dir.magnitudeVariation

    Emitter {id: emitter; anchors.fill: parent; velocity: AngleDirection{id: dir}}
    ImageParticle {id: painter; anchors.fill: parent}

    property Affector motion: null
    onMotionChanged: motion.setParent(container);//Might not clean up the old ones as well, but they should be deleted by the user

    function burst(count, rate) {
        //Behaviour change: Now sets emission rate for all instead of being separate
        if(rate != undefined && rate != -1) {
            emitter.emitRate = rate;
            emitter.pulse(count / rate);
        } else {
            emitter.burst(count);
        }
    }
}

