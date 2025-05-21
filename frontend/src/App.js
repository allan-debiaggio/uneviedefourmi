import React, { useRef, useState, useEffect } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, useTexture } from '@react-three/drei';
import * as THREE from 'three';
import bgImage from './assets/image.jpg';

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

const menuButtonStyle = {
  background: 'linear-gradient(145deg, #1de9b6 60%, #00bfae 100%)',
  color: '#fff',
  border: 'none',
  borderRadius: '50%',
  boxShadow: '0 6px 20px #00bfae88, 0 1.5px 0 #fff inset',
  fontSize: 22,
  fontWeight: 700,
  padding: 0,
  margin: '18px 0',
  cursor: 'pointer',
  transition: 'transform 0.2s, box-shadow 0.2s, filter 0.2s, opacity 0.2s',
  textShadow: '0 2px 8px #0008',
  letterSpacing: 1.5,
  outline: 'none',
  minWidth: 0,
  filter: 'drop-shadow(0 0 8px #00bfae88)',
  opacity: 0.5,
};

const menuButtonHoverStyle = {
  filter: 'drop-shadow(0 0 24px #00fff7cc) brightness(1.15)',
  transform: 'scale(1.18) translateY(-10px)',
  opacity: 0.95,
  zIndex: 10,
};

function MenuPage({ onSelect }) {
  const [hovered, setHovered] = useState(-1);
  const buttonCount = 6;
  const buttonBaseSize = 56;
  const radius = 180;
  const [center, setCenter] = useState({ x: 0, y: 0 });
  const [positions, setPositions] = useState([]);

  useEffect(() => {
    function updatePositions() {
      const x = window.innerWidth / 2;
      const y = window.innerHeight / 2 + 40;
      setCenter({ x, y });
      const pos = [];
      for (let idx = 0; idx < buttonCount; ++idx) {
        const angle = (idx / buttonCount) * 2 * Math.PI - Math.PI / 2;
        const px = x + radius * Math.cos(angle) - buttonBaseSize / 2;
        const py = y + radius * Math.sin(angle) - buttonBaseSize / 2;
        pos.push({ left: px, top: py });
      }
      setPositions(pos);
    }
    updatePositions();
    window.addEventListener('resize', updatePositions);
    return () => window.removeEventListener('resize', updatePositions);
  }, []);

  return (
    <div
      style={{
        width: '100vw',
        minHeight: '100vh',
        background: `linear-gradient(120deg, #0f2027 0%, #2c5364 100%)`,
        position: 'relative',
        overflow: 'hidden',
      }}
    >
      <div
        style={{
          position: 'absolute',
          top: 0,
          left: 0,
          width: '100vw',
          height: '100vh',
          zIndex: 0,
          backgroundImage: `url(${bgImage})`,
          backgroundRepeat: 'no-repeat',
          backgroundPosition: 'center center',
          backgroundSize: 'auto 90vh',
          pointerEvents: 'none',
        }}
      />
      <div style={{ position: 'relative', zIndex: 1, width: '100vw', height: '100vh' }}>
        <h1 style={{
          color: '#fff',
          fontSize: 38,
          fontWeight: 900,
          textShadow: '0 4px 32px #00bfae, 0 2px 8px #000',
          marginBottom: 40,
          letterSpacing: 2,
          background: 'linear-gradient(145deg, #1de9b6 60%, #00bfae 100%)',
          borderRadius: 18,
          padding: '16px 40px',
          width: 'fit-content',
          marginLeft: 'auto',
          marginRight: 'auto',
          boxShadow: '0 6px 20px #00bfae88, 0 1.5px 0 #fff inset',
          filter: 'drop-shadow(0 0 8px #00bfae88)',
        }}>
          Fourmilières 3D – Simulation
        </h1>
        <div style={{ position: 'absolute', left: 0, top: 0, width: '100vw', height: '100vh', pointerEvents: 'none' }}>
          {positions.map((pos, idx) => {
            const isHovered = hovered === idx;
            return (
              <button
                key={idx}
                style={{
                  ...menuButtonStyle,
                  position: 'absolute',
                  left: pos.left,
                  top: pos.top,
                  width: buttonBaseSize,
                  height: buttonBaseSize,
                  fontSize: isHovered ? 22 : 16,
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'center',
                  ...(isHovered ? menuButtonHoverStyle : {}),
                  pointerEvents: 'auto',
                  padding: 0,
                  lineHeight: 1,
                  letterSpacing: 0.5,
                }}
                onMouseEnter={() => setHovered(idx)}
                onMouseLeave={() => setHovered(-1)}
                onClick={() => onSelect(idx + 1)}
              >
                {`F${idx + 1}`}
              </button>
            );
          })}
        </div>
      </div>
    </div>
  );
}

// --- Fourmiliere 2 (F2) ---
const roomPositionsF2 = {
  Sv: [0, 0, 0],
  S1: [2, 1, 0],
  S2: [2, -1, 0],
  Sd: [4, 0, 0],
};
const roomsF2 = [
  { id: 'Sv', color: 'green' },
  { id: 'S1', color: 'blue' },
  { id: 'S2', color: 'blue' },
  { id: 'Sd', color: 'red' },
];
const edgesF2 = [
  ['Sv', 'S1'],
  ['Sv', 'S2'],
  ['S1', 'Sd'],
  ['S2', 'Sd'],
];
const pathsF2 = [
  ['Sv', 'S1', 'Sd'],
  ['Sv', 'S2', 'Sd'],
];
const initialAntsF2 = [
  { id: 1, path: 0, position: 0, progress: 0, moving: false, moveStartTime: null },
  { id: 2, path: 1, position: 0, progress: 0, moving: false, moveStartTime: null },
];

function AntF2({ ant }) {
  const path = pathsF2[ant.path];
  const from = roomPositionsF2[path[ant.position]];
  const to = ant.position < path.length - 1 ? roomPositionsF2[path[ant.position + 1]] : from;
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

function FourmiliereSceneF2({ ants }) {
  return (
    <Canvas camera={{ position: [2, 2, 8], fov: 50 }} shadows>
      <ambientLight intensity={0.7} />
      <directionalLight position={[10, 10, 10]} intensity={0.7} castShadow />
      {/* sol */}
      <mesh position={[2, -1.5, 0]} receiveShadow>
        <boxGeometry args={[7, 0.1, 5]} />
        <meshStandardMaterial color="#225522" />
      </mesh>
      {/* rooms */}
      {roomsF2.map((room) => (
        <mesh key={room.id} position={roomPositionsF2[room.id]} castShadow>
          <sphereGeometry args={[0.5, 32, 32]} />
          <meshStandardMaterial color={room.color} />
        </mesh>
      ))}
      {/* edges */}
      {edgesF2.map(([from, to], i) => {
        const p1 = roomPositionsF2[from];
        const p2 = roomPositionsF2[to];
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
        <AntF2 key={ant.id} ant={ant} />
      ))}
      <OrbitControls />
    </Canvas>
  );
}

// --- Fourmiliere 3 (F3) ---
const roomPositionsF3 = {
  Sv: [0, 0.5, 0],
  S1: [2, 1.7, 0],
  S2: [4, 0.5, 0],
  Sd: [2, -0.7, 0],
};
const roomsF3 = [
  { id: 'Sv', color: 'green' },
  { id: 'S1', color: 'blue' },
  { id: 'S2', color: 'blue' },
  { id: 'Sd', color: 'red' },
];
const edgesF3 = [
  ['Sv', 'S1'],
  ['S1', 'S2'],
  ['S2', 'Sd'],
  ['Sd', 'Sv'], // cycle edge
];
const pathF3 = ['Sv', 'S1', 'S2', 'Sd'];
const initialAntsF3 = Array.from({ length: 5 }, (_, i) => ({
  id: i + 1,
  position: 0, // index in pathF3
  progress: 0,
  moving: false,
  moveStartTime: null,
}));

function AntF3({ ant }) {
  const from = roomPositionsF3[pathF3[ant.position]];
  const to = ant.position < pathF3.length - 1 ? roomPositionsF3[pathF3[ant.position + 1]] : from;
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

function FourmiliereSceneF3({ ants }) {
  return (
    <Canvas camera={{ position: [2, 2, 8], fov: 50 }} shadows>
      <ambientLight intensity={0.7} />
      <directionalLight position={[10, 10, 10]} intensity={0.7} castShadow />
      {/* sol */}
      <mesh position={[2, -1.5, 0]} receiveShadow>
        <boxGeometry args={[7, 0.1, 5]} />
        <meshStandardMaterial color="#225522" />
      </mesh>
      {/* rooms */}
      {roomsF3.map((room) => (
        <mesh key={room.id} position={roomPositionsF3[room.id]} castShadow>
          <sphereGeometry args={[0.5, 32, 32]} />
          <meshStandardMaterial color={room.color} />
        </mesh>
      ))}
      {/* edges */}
      {edgesF3.map(([from, to], i) => {
        const p1 = roomPositionsF3[from];
        const p2 = roomPositionsF3[to];
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
            <lineBasicMaterial color={i === 3 ? '#00fff7' : 'white'} linewidth={2} />
          </line>
        );
      })}
      {/* ants */}
      {ants.map((ant) => (
        <AntF3 key={ant.id} ant={ant} />
      ))}
      <OrbitControls />
    </Canvas>
  );
}

export default function App() {
  const [page, setPage] = useState('menu'); // 'menu' ou numéro de fourmilière
  const [ants, setAnts] = useState(initialAnts);
  const [step, setStep] = useState(0);
  const [running, setRunning] = useState(false);
  const [hasStarted, setHasStarted] = useState(false);
  const intervalRef = useRef();

  // F2 state
  const [antsF2, setAntsF2] = useState(initialAntsF2.map(a => ({ ...a })));
  const [stepF2, setStepF2] = useState(0);
  const [runningF2, setRunningF2] = useState(false);
  const [hasStartedF2, setHasStartedF2] = useState(false);
  const intervalRefF2 = useRef();

  // F3 state
  const [antsF3, setAntsF3] = useState(initialAntsF3.map(a => ({ ...a })));
  const [stepF3, setStepF3] = useState(0);
  const [runningF3, setRunningF3] = useState(false);
  const [hasStartedF3, setHasStartedF3] = useState(false);
  const intervalRefF3 = useRef();

  // Animation logic with controls
  useEffect(() => {
    if (!running) return;
    intervalRef.current = setInterval(() => {
      setAnts((prevAnts) => {
        const occupation = { S1: null, S2: null };
        prevAnts.forEach((ant) => {
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
              const nextFree =
                nextRoom === 'Sd' ||
                (nextRoom === 'S1' && occupation.S1 === null) ||
                (nextRoom === 'S2' && occupation.S2 === null);
              if (nextFree) {
                ant.moving = true;
                ant.moveStartTime = now;
                if (path[ant.position] === 'S1') occupation.S1 = null;
                if (path[ant.position] === 'S2') occupation.S2 = null;
                if (nextRoom === 'S1') occupation.S1 = ant.id;
                if (nextRoom === 'S2') occupation.S2 = ant.id;
              }
            } else {
              const elapsed = (now - ant.moveStartTime) / 1000;
              ant.progress = Math.min(elapsed / 1, 1);
              if (ant.progress >= 1) {
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
    return () => clearInterval(intervalRef.current);
  }, [running]);

  // F2 animation logic
  useEffect(() => {
    if (!runningF2) return;
    intervalRefF2.current = setInterval(() => {
      setAntsF2((prevAnts) => {
        // occupation[0] for S1, occupation[1] for S2
        const occupation = [null, null];
        prevAnts.forEach((ant) => {
          const path = pathsF2[ant.path];
          if (ant.moving && ant.position < path.length - 1) {
            const nextRoom = path[ant.position + 1];
            if (nextRoom === 'S1') occupation[0] = ant.id;
            if (nextRoom === 'S2') occupation[1] = ant.id;
          } else {
            if (ant.position === 1 && !ant.moving && ant.path === 0) occupation[0] = ant.id;
            if (ant.position === 1 && !ant.moving && ant.path === 1) occupation[1] = ant.id;
          }
        });
        const now = Date.now();
        const newAnts = [...prevAnts];
        for (let i = 0; i < newAnts.length; ++i) {
          const ant = newAnts[i];
          const path = pathsF2[ant.path];
          if (ant.position < path.length - 1) {
            const nextRoom = path[ant.position + 1];
            const occIdx = ant.path; // 0 for S1, 1 for S2
            if (!ant.moving) {
              const nextFree = nextRoom === 'Sd' || occupation[occIdx] === null;
              if (nextFree) {
                ant.moving = true;
                ant.moveStartTime = now;
                if (path[ant.position] === 'S1' && occIdx === 0) occupation[0] = null;
                if (path[ant.position] === 'S2' && occIdx === 1) occupation[1] = null;
                if (nextRoom === 'S1') occupation[0] = ant.id;
                if (nextRoom === 'S2') occupation[1] = ant.id;
              }
            } else {
              const elapsed = (now - ant.moveStartTime) / 1000;
              ant.progress = Math.min(elapsed / 1, 1);
              if (ant.progress >= 1) {
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
      setStepF2((s) => s + 1);
    }, 30);
    return () => clearInterval(intervalRefF2.current);
  }, [runningF2]);

  // F3 animation logic
  useEffect(() => {
    if (!runningF3) return;
    intervalRefF3.current = setInterval(() => {
      setAntsF3((prevAnts) => {
        // occupation: 0=Sv, 1=S1, 2=S2, 3=Sd
        const occupation = [null, null, null, null];
        prevAnts.forEach((ant) => {
          if (ant.moving && ant.position < pathF3.length - 1) {
            const nextRoomIdx = ant.position + 1;
            if (nextRoomIdx === 1) occupation[1] = ant.id;
            if (nextRoomIdx === 2) occupation[2] = ant.id;
          } else {
            if (ant.position === 1 && !ant.moving) occupation[1] = ant.id;
            if (ant.position === 2 && !ant.moving) occupation[2] = ant.id;
          }
        });
        const now = Date.now();
        const newAnts = [...prevAnts];
        for (let i = 0; i < newAnts.length; ++i) {
          const ant = newAnts[i];
          if (ant.position < pathF3.length - 1) {
            const nextRoomIdx = ant.position + 1;
            const nextRoom = pathF3[nextRoomIdx];
            if (!ant.moving) {
              const nextFree = nextRoom === 'Sd' || occupation[nextRoomIdx] === null;
              if (nextFree) {
                ant.moving = true;
                ant.moveStartTime = now;
                if (ant.position === 1) occupation[1] = null;
                if (ant.position === 2) occupation[2] = null;
                if (nextRoomIdx === 1) occupation[1] = ant.id;
                if (nextRoomIdx === 2) occupation[2] = ant.id;
              }
            } else {
              const elapsed = (now - ant.moveStartTime) / 1000;
              ant.progress = Math.min(elapsed / 1, 1);
              if (ant.progress >= 1) {
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
      setStepF3((s) => s + 1);
    }, 30);
    return () => clearInterval(intervalRefF3.current);
  }, [runningF3]);

  // Start button handler
  const handleStart = () => {
    setHasStarted(true);
    setRunning(true);
  };
  // Resume button handler
  const handleResume = () => setRunning(true);
  // Replay button handler
  const handleReplay = () => {
    setAnts(initialAnts.map(a => ({ ...a }))); // reset ants
    setStep(0);
    setHasStarted(false);
    setRunning(false);
  };
  // Pause button handler
  const handlePause = () => setRunning(false);

  // F2 controls
  const handleStartF2 = () => {
    setHasStartedF2(true);
    setRunningF2(true);
  };
  const handleResumeF2 = () => setRunningF2(true);
  const handleReplayF2 = () => {
    setAntsF2(initialAntsF2.map(a => ({ ...a })));
    setStepF2(0);
    setHasStartedF2(false);
    setRunningF2(false);
  };
  const handlePauseF2 = () => setRunningF2(false);

  // F3 controls
  const handleStartF3 = () => {
    setHasStartedF3(true);
    setRunningF3(true);
  };
  const handleResumeF3 = () => setRunningF3(true);
  const handleReplayF3 = () => {
    setAntsF3(initialAntsF3.map(a => ({ ...a })));
    setStepF3(0);
    setHasStartedF3(false);
    setRunningF3(false);
  };
  const handlePauseF3 = () => setRunningF3(false);
  const handleReturn = () => setPage('menu');

  if (page === 'menu') {
    return <MenuPage onSelect={(n) => setPage(n)} />;
  }

  // Pour l'instant, seule la fourmilière 1 est fonctionnelle
  if (page === 1) {
    return (
      <div style={{ width: '100vw', height: '100vh', background: 'linear-gradient(#a3d9a5, #e0ffe0)' }}>
        <h2 style={{ position: 'absolute', left: 20, top: 10, color: '#234', zIndex: 10 }}>
          Fourmilière 1 – Simulation (React + Three.js)
        </h2>
        <div style={{ position: 'absolute', left: 20, top: 100, zIndex: 10, display: 'flex', gap: 10 }}>
          {!hasStarted && (
            <button onClick={handleStart} style={buttonStyle} title="Start">
              <span role="img" aria-label="start">▶️</span>
            </button>
          )}
          {hasStarted && running && (
            <button onClick={handlePause} style={buttonStyle} title="Pause">
              <span role="img" aria-label="pause">⏸️</span>
            </button>
          )}
          {hasStarted && !running && (
            <button onClick={handleResume} style={buttonStyle} title="Resume">
              <span role="img" aria-label="resume">▶️</span>
            </button>
          )}
          <button onClick={handleReplay} style={buttonStyle} title="Replay">
            <span role="img" aria-label="replay">🔄</span>
          </button>
          <button onClick={handleReturn} style={{ ...buttonStyle, background: '#222', color: '#fff', fontSize: 18, width: 48, height: 48 }} title="Return">
            <span role="img" aria-label="return">↩️</span>
          </button>
        </div>
        <FourmiliereScene ants={ants} step={step} />
      </div>
    );
  }
  if (page === 2) {
    return (
      <div style={{ width: '100vw', height: '100vh', background: 'linear-gradient(#a3d9a5, #e0ffe0)' }}>
        <h2 style={{ position: 'absolute', left: 20, top: 10, color: '#234', zIndex: 10 }}>
          Fourmilière 2 – Simulation (React + Three.js)
        </h2>
        <div style={{ position: 'absolute', left: 20, top: 100, zIndex: 10, display: 'flex', gap: 10 }}>
          {!hasStartedF2 && (
            <button onClick={handleStartF2} style={buttonStyle} title="Start">
              <span role="img" aria-label="start">▶️</span>
            </button>
          )}
          {hasStartedF2 && runningF2 && (
            <button onClick={handlePauseF2} style={buttonStyle} title="Pause">
              <span role="img" aria-label="pause">⏸️</span>
            </button>
          )}
          {hasStartedF2 && !runningF2 && (
            <button onClick={handleResumeF2} style={buttonStyle} title="Resume">
              <span role="img" aria-label="resume">▶️</span>
            </button>
          )}
          <button onClick={handleReplayF2} style={buttonStyle} title="Replay">
            <span role="img" aria-label="replay">🔄</span>
          </button>
          <button onClick={handleReturn} style={{ ...buttonStyle, background: '#222', color: '#fff', fontSize: 18, width: 48, height: 48 }} title="Return">
            <span role="img" aria-label="return">↩️</span>
          </button>
        </div>
        <FourmiliereSceneF2 ants={antsF2} />
      </div>
    );
  }
  if (page === 3) {
    return (
      <div style={{ width: '100vw', height: '100vh', background: 'linear-gradient(#a3d9a5, #e0ffe0)' }}>
        <h2 style={{ position: 'absolute', left: 20, top: 10, color: '#234', zIndex: 10 }}>
          Fourmilière 3 – Simulation (React + Three.js)
        </h2>
        <div style={{ position: 'absolute', left: 20, top: 100, zIndex: 10, display: 'flex', gap: 10 }}>
          {!hasStartedF3 && (
            <button onClick={handleStartF3} style={buttonStyle} title="Start">
              <span role="img" aria-label="start">▶️</span>
            </button>
          )}
          {hasStartedF3 && runningF3 && (
            <button onClick={handlePauseF3} style={buttonStyle} title="Pause">
              <span role="img" aria-label="pause">⏸️</span>
            </button>
          )}
          {hasStartedF3 && !runningF3 && (
            <button onClick={handleResumeF3} style={buttonStyle} title="Resume">
              <span role="img" aria-label="resume">▶️</span>
            </button>
          )}
          <button onClick={handleReplayF3} style={buttonStyle} title="Replay">
            <span role="img" aria-label="replay">🔄</span>
          </button>
          <button onClick={handleReturn} style={{ ...buttonStyle, background: '#222', color: '#fff', fontSize: 18, width: 48, height: 48 }} title="Return">
            <span role="img" aria-label="return">↩️</span>
          </button>
        </div>
        <FourmiliereSceneF3 ants={antsF3} />
      </div>
    );
  }
  // Placeholder pour les autres fourmilières
  return (
    <div style={{ width: '100vw', height: '100vh', background: '#222', color: '#fff', display: 'flex', alignItems: 'center', justifyContent: 'center', fontSize: 32 }}>
      Fourmilière {page} – Coming soon!
      <button style={{ ...menuButtonStyle, marginLeft: 40 }} onClick={() => setPage('menu')}>Back to menu</button>
    </div>
  );
}

// Style pour les boutons verts arrondis
const buttonStyle = {
  background: 'linear-gradient(90deg, #4caf50 60%, #81c784 100%)',
  color: 'white',
  border: 'none',
  borderRadius: '50%',
  width: 48,
  height: 48,
  fontSize: 28,
  boxShadow: '0 2px 8px #0002',
  cursor: 'pointer',
  outline: 'none',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  transition: 'background 0.2s',
};
