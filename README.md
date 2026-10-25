# Map Fight

## Description
**Map Fight** is a C++ Object-Oriented battle simulation inspired by "The Battle of Polytopia". Currently in Phase 1, the project focuses on core backend architecture, memory management, and class interactions, laying the foundation for a future grid-based strategy game.

The engine currently supports dynamically loading faction units from external text files, army composition, and text-based tactical combat resolution.

## Factions & Lore
The game engine supports distinct thematic tribes, dynamically loaded into the simulation:
1. **Dwarfs:** Focused on high armor and brute force.
2. **Demons:** Focused on devastating attacks.
3. **Pirates:** Focused on ranged firepower and mobility.
4. **Sand People:** Balanced desert warriors.

## Instrucțiuni de compilare
**1. Pasul de configurare**
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh co