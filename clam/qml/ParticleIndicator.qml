import QtQuick.Particles 2.0

ParticleSystem {
    id: system
    property int max: 1
    property int done: 0
    ImageParticle {
        groups: "undone"
        source: "particle.png"
        color: "#00CC3333"
        colorVariation: 0.2
    }
    ImageParticle {
        groups: "done"
        source: "particle.png"
        color: "#0033CC33"
        colorVariation: 0.2
    }
    Emitter {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.1
        group: "undecided"
        lifeSpan: 4000
        emitRate: 20
        size: 32
        velocity: PointDirection { id: pv; x: 0; y: system.height / -2 }
        acceleration: PointDirection { id: pa; xVariation: 20; y: pv.y * -0.3; yVariation: pa.y * 0.03 }
    }
    ParticleGroup {
        name: "undecided"
        duration: 0
        to: { "done" : done, "undone" : max - done }
    }
}
