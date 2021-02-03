import QtQuick 2.3

Canvas {
    required property int block_size
    required property int size
    property var map
    property var explored

    id: field
    width: size * block_size + size - 1
    height: size * block_size + size - 1

    function calcCoord(i) {
        return i * block_size + i;
    }

    function shadeBlock(ctx, x, y, w, rgba) {
        // ctx.clearRect(x, y, w, w);
        ctx.fillStyle = rgba;
        ctx.fillRect(x, y, w, w);
    }

    onPaint: {
        console.log('render!');
        const ALPHA = 0.1;
        let ctx = getContext("2d");

        // Draw background
        for (let i = 0; i < size; i++) {
            for (let j = 0; j < size; j++) {
                ctx.drawImage('/img/background.png', calcCoord(i), calcCoord(j), block_size, block_size);
                if (!(`${i};${j}` in explored))
                    shadeBlock(ctx, calcCoord(i), calcCoord(j), block_size, Qt.rgba(0, 0, 0, 0.2));
            }
        }
        
        // Draw objects
        for (let obj of map) {
            const x = calcCoord(obj.pos.x);
            const y = calcCoord(obj.pos.y);

            if (obj.hasOwnProperty('controller_radius') || obj.hasOwnProperty('sensor_radius')) {
                const color = obj.hasOwnProperty('controller_radius') ? Qt.rgba(0, 0, 255, ALPHA) : Qt.rgba(255, 0, 0, ALPHA);
                const radius = obj['controller_radius'] || obj['sensor_radius'];

                for (let i = Math.max(0, obj.pos.x - radius); i <= Math.min(obj.pos.x + radius, width - 1); i++) {
                    const hRadius = radius - Math.abs(i - obj.pos.x);
                    for (let j = Math.max(0, obj.pos.y - hRadius); j <= Math.min(obj.pos.y + hRadius, height - 1); j++) {
                        shadeBlock(ctx, calcCoord(i), calcCoord(j), block_size, color);
                    }
                }
            }

            ctx.drawImage(`/img/${obj.type}.png`, x, y, block_size, block_size);
        }
    }

    Component.onCompleted: {
        loadImage("/img/background.png");
        loadImage("/img/command-center.png");
        loadImage("/img/interest-point.png");
        loadImage("/img/obstacle.png");
        loadImage("/img/robot.png");
        loadImage("/img/robot-commander.png");
    }

    onMapChanged: requestPaint();
}
