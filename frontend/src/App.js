import React, { useRef, useState, useEffect } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, useTexture } from '@react-three/drei';
import * as THREE from 'three';

// Positions des salles (mock, linéaire)
const roomPositions = {
  Sv: [0, 0, 0],
  S1: [2, 0, 0],
  S2: [4, 0, 0],
  Sd: [6, 0, 0],
};

const rooms = [
  { id: 'Sv', color: 'green' },
  { id: 'S1', color: 'blue' },
  { id: 'S2', color: 'blue' },
  { id: 'Sd', color: 'red' },
];

const edges = [
  ['Sv', 'S1'],
  ['S1', 'S2'],
  ['S2', 'Sd'],
];

// Ants initial state: all at Sv
const initialAnts = Array.from({ length: 5 }, (_, i) => ({
  id: i + 1,
  position: 0, // index in path
  progress: 0,
  moving: false,
  moveStartTime: null, // timestamp du début du mouvement
}));

const path = ['Sv', 'S1', 'S2', 'Sd'];

function Ant({ ant, step }) {
  // Interpolate position between rooms for animation
  const from = roomPositions[path[ant.position]];
  const to =
    ant.position < path.length - 1
      ? roomPositions[path[ant.position + 1]]
      : from;
  // Animation progress (0 to 1)
  const progress = ant.progress || 0;
  const pos = [
    from[0] + (to[0] - from[0]) * progress,
    from[1] + (to[1] - from[1]) * progress,
    from[2] + (to[2] - from[2]) * progress,
  ];
  return (
    <mesh position={pos}>
      <sphereGeometry args={[0.25, 32, 32]} />
      <meshStandardMaterial color="red" emissive="red" />
    </mesh>
  );
}

function FourmiliereScene({ ants, step }) {
  // Forêt : simple plan texturé ou couleur verte
  // (pour un vrai fond forêt, il faudrait une texture ou un HDRI)
  return (
    <Canvas camera={{ position: [3, 3, 10], fov: 50 }} shadows>
      <ambientLight intensity={0.7} />
      <directionalLight position={[10, 10, 10]} intensity={0.7} castShadow />
      {/* sol */}
      <mesh position={[3, -1, 0]} receiveShadow>
        <boxGeometry args={[10, 0.1, 6]} />
        <meshStandardMaterial color="#225522" />
      </mesh>
      {/* rooms */}
      {rooms.map((room) => (
        <mesh key={room.id} position={roomPositions[room.id]} castShadow>
          <sphereGeometry args={[0.5, 32, 32]} />
          <meshStandardMaterial color={room.color} />
        </mesh>
      ))}
      {/* edges */}
      {edges.map(([from, to], i) => {
        const p1 = roomPositions[from];
        const p2 = roomPositions[to];
        const points = [new THREE.Vector3(...p1), new THREE.Vector3(...p2)];
        return (
          <line key={i}>
            <bufferGeometry>
              <bufferAttribute
                attach="attributes-position"
                count={2}
                array={new Float32Array([...p1, ...p2])}
                itemSize={3}
              />
            </bufferGeometry>
            <lineBasicMaterial color="white" linewidth={2} />
          </line>
        );
      })}
      {/* ants */}
      {ants.map((ant) => (
        <Ant key={ant.id} ant={ant} step={step} />
      ))}
      <OrbitControls />
    </Canvas>
  );
}

function useAntsAnimation() {
  const [ants, setAnts] = useState(initialAnts);
  const [step, setStep] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => {
      setAnts((prevAnts) => {
        // Occupation dynamique : inclut les fourmis en mouvement vers la salle suivante
        const occupation = { S1: null, S2: null };
        prevAnts.forEach((ant) => {
          // Si une fourmi est en train d'arriver dans une salle, on la considère comme occupant la salle cible
          if (ant.moving && ant.position < path.length - 1) {
            const nextRoom = path[ant.position + 1];
            if (nextRoom === 'S1') occupation.S1 = ant.id;
            if (nextRoom === 'S2') occupation.S2 = ant.id;
          } else {
            if (ant.position === 1 && !ant.moving) occupation.S1 = ant.id;
            if (ant.position === 2 && !ant.moving) occupation.S2 = ant.id;
          }
        });
        const now = Date.now();
        const newAnts = [...prevAnts];
        for (let i = 0; i < newAnts.length; ++i) {
          const ant = newAnts[i];
          if (ant.position < path.length - 1) {
            const nextRoom = path[ant.position + 1];
            if (!ant.moving) {
              // Vérifier si la salle suivante est libre (Sd toujours libre)
              const nextFree =
                nextRoom === 'Sd' ||
                (nextRoom === 'S1' && occupation.S1 === null) ||
                (nextRoom === 'S2' && occupation.S2 === null);
              if (nextFree) {
                ant.moving = true;
                ant.moveStartTime = now;
                // Libérer la salle actuelle
                if (path[ant.position] === 'S1') occupation.S1 = null;
                if (path[ant.position] === 'S2') occupation.S2 = null;
                // Occuper la salle cible dès le début du mouvement
                if (nextRoom === 'S1') occupation.S1 = ant.id;
                if (nextRoom === 'S2') occupation.S2 = ant.id;
              }
            } else {
              // Animation fluide sur 1 seconde
              const elapsed = (now - ant.moveStartTime) / 1000;
              ant.progress = Math.min(elapsed / 1, 1);
              if (ant.progress >= 1) {
                // Arrivé dans la nouvelle salle
                ant.position++;
                ant.progress = 0;
                ant.moving = false;
                ant.moveStartTime = null;
              }
            }
          }
        }
        return newAnts.map((a) => ({ ...a }));
      });
      setStep((s) => s + 1);
    }, 30);
    return () => clearInterval(interval);
  }, []);

  return [ants, step];
}

export default function App() {
  const [ants, step] = useAntsAnimation();
  return (
    <div style={{ width: '100vw', height: '100vh', background: 'linear-gradient(#a3d9a5, #e0ffe0)' }}>
      <h2 style={{ position: 'absolute', left: 20, top: 10, color: '#234', zIndex: 10 }}>
        Fourmilière 1 – Simulation (React + Three.js)
      </h2>
      <FourmiliereScene ants={ants} step={step} />
    </div>
  );
}
