# 射击游戏项目改进待办清单

## 核心功能缺陷 (P0 - 必须修复)

- [ ] **实现碰撞检测系统**
  - 文件: `src/Entity/CollisionManager.cpp`
  - 任务: 完成 `Update()` 方法，实现 AABB 碰撞检测
  - 建议: 先实现简单的 O(N²) 检测，实体多时再优化空间分割

---

## 内存安全 (P1 - 高优先级)

- [ ] **EntityConfig 所有权重构**
  - 文件: `src/Entity/EntityManager.cpp`, `src/Entity/EntityConfig.h`
  - 任务: 改用 `std::unique_ptr` 或传 const 引用，避免所有权混淆

- [ ] **BehaviorComponent 使用智能指针**
  - 文件: `src/Entity/BehaviorComponent.h/cpp`
  - 任务: `std::vector<std::unique_ptr<BaseBehavior>> behaviors`

- [ ] **EntityManager 组件存储改用智能指针**
  - 文件: `src/Entity/EntityManager.h/cpp`
  - 任务: `std::unordered_map<int, std::unique_ptr<Entity>>`

---

## 游戏循环优化 (P1 - 高优先级)

- [ ] **引入 Delta Time**
  - 文件: `src/game.cpp`
  - 任务: 使用 `std::chrono` 计算帧间隔，传递给更新逻辑
  - 涉及: `Game::Run()`, `Game::LogicUpdate()`

- [ ] **移除固定延迟**
  - 文件: `src/game.cpp`
  - 任务: 替换 `SDL_Delay(1000 / FRAME_RATE)` 为基于时间的帧率控制

---

## 架构解耦 (P2 - 中等优先级)

- [ ] **引入 GameContext 上下文**
  - 新文件: `src/Core/GameContext.h`
  - 任务: 集中管理所有管理器引用，替代单例模式

- [ ] **Scene 接口改造**
  - 文件: `src/Scene/Scene.h`, 各 Scene 实现
  - 任务: `Update(GameContext& ctx)`, `Init()` 返回 bool

- [ ] **逐步替换单例调用**
  - 涉及文件: `GameScene.cpp`, `EntityManager`, `RenderManager`, `CollisionManager`
  - 任务: 通过上下文或依赖注入传递管理器引用

---

## 渲染性能 (P2 - 中等优先级)

- [ ] **优化 RenderManager 排序**
  - 文件: `src/Entity/RenderManager.cpp`
  - 任务: 添加 `needsSorting` 标志，仅在组件增删时排序

- [ ] **批量渲染支持**
  - 文件: `src/Entity/RenderManager.cpp`
  - 任务: 按纹理分组批量提交，减少 GPU 状态切换

---

## 事件系统完善 (P2 - 中等优先级)

- [ ] **修复事件处理器安全问题**
  - 文件: `src/Event/GameEvent.h`
  - 任务: `emit()` 时复制 handlers，防止遍历时修改

- [ ] **添加事件优先级/队列**
  - 文件: `src/Event/EventManager.h`
  - 任务: 支持延迟事件和优先级处理

---

## UI 系统改进 (P2 - 中等优先级)

- [ ] **分离 Canvas 渲染与事件处理**
  - 文件: `src/GUI/UI/Canvas.cpp`
  - 任务: 拆分为 `Render()` 和 `ProcessEvents()` 两个方法

- [ ] **统一 UI 更新接口**
  - 文件: `src/GUI/UI/UIComponent.h`, `src/GUI/UpdateContext.h`
  - 任务: 移除 `InternalUpdate` 的空 events 向量 hack

---

## 代码质量 (P3 - 低优先级)

- [ ] **组件内存布局优化**
  - 文件: `src/Entity/Entity.h`
  - 任务: 考虑使用 SoA (Structure of Arrays) 替代 AoS

- [ ] **添加单元测试框架**
  - 任务: 引入测试框架 (如 doctest)，为核心系统添加测试

- [ ] **完善错误处理**
  - 文件: `src/Scene/SceneManager.cpp`
  - 任务: `ChangeScene` 添加错误处理和资源回滚

- [ ] **代码注释和文档**
  - 任务: 为公共接口添加 Doxygen 风格注释

---

## 开发工具 (P3 - 低优先级)

- [ ] **添加性能分析器**
  - 任务: 集成简单的帧时间/调用次数统计

- [ ] **热重载资源**
  - 文件: `src/Asset/AssetManager.cpp`
  - 任务: 开发模式下支持运行时重新加载纹理

---

## 快速开始建议

如果你打算开始改进，建议按以下顺序：

1. **先修 P0**: 实现碰撞检测（否则游戏没有核心玩法）
2. **再修 P1**: 引入 Delta Time 和智能指针（提升稳定性）
3. **逐步推进 P2**: 架构优化可以边开发边改
