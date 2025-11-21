# 🐦 Boid Simulation in C++ (SDL2)

This project is an interactive **boid / flocking simulation** written in **C++** using **SDL2**.  
It showcases object-oriented programming, inheritance, templates, exceptions, and RTTI, while simulating emergent flocking behavior inspired by Craig Reynolds' *Boids* model.

The simulation renders three types of boids, each with distinct behavior:

- **Boid** → standard flocking behavior  
- **CharBoid** → attracted to the mouse  
- **PieBoid** → repelled by the mouse  

Made as part of a C++ Athens course final assignment.

---

## 🎥 Demo Video

https://github.com/user-attachments/assets/6c589912-815a-4584-a111-d421c929d925

---

## 🚀 Features

| Feature | Description |
|---------|------------|
| **Flocking Simulation** | Cohesion, alignment, separation rules implemented manually |
| **Polymorphism & Inheritance** | `Boid` base class with `CharBoid` and `PieBoid` subclasses |
| **RTTI (`dynamic_cast`)** | Used to apply different behaviors based on runtime type |
| **Exception Handling** | Custom `BoidException` for safe object creation |
| **Templates** | `RandomRange<T>` class template for typed random generation |
| **Wrapping Movement** | Boids seamlessly wrap around screen edges |
| **Mouse Interaction** | Live player influence on flock behavior |

---

## 🛠 Tech Stack

- **C++17**
- **SDL2**
- **STL**
- Compiler: *g++, clang++, or MSVC*


---

## 🧠 How the Logic Works

### **Flocking Rules**
Each boid computes neighbors and applies the standard algorithm:

- *Cohesion* → move toward average position  
- *Alignment* → match velocity  
- *Separation* → avoid crowding

### **RTTI Behavior**
Runtime polymorphism determines special actions:

```cpp
if (auto* cb = dynamic_cast<CharBoid*>(b.get())) {
    // Move toward mouse
}
else if (auto* pb = dynamic_cast<PieBoid*>(b.get())) {
    // Move away from mouse
}
```
