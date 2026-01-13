# Refactoring Plan - PowerPlus3 Project

This document outlines the overall refactoring plan for the PowerPlus3 project, divided into phases for easier management and execution.

---

## Overview

**Objective**: Improve code quality, increase consistency, and adhere to modern C++ best practices.

**Approach**: Break down into phases, with each phase focusing on a specific goal.

---

## Phase 1: Naming Convention Conversion

### Objective

Convert from the current naming conventions to **Google C++ Style Guide** naming conventions.

### Scope

- Functions: `PascalCase` → `camelCase`
- Member variables: `m_camelCase` → `camelCase_`
- Local variables: `camelCase` → `camelCase` (unchanged)
- Function parameters: `camelCase` → `camelCase` (unchanged)
- Constants: `UPPER_CASE` → `kConstantName`
- Namespaces: `PascalCase` → `snake_case`
- Global variables: `g_` + Hungarian → `g_` + `camelCase_`
- Structs: `UPPER_CASE` → `PascalCase` (if applicable)

### Reference Documents

- [GOOGLE_NAMING_CONVERSION_GUIDE.md](./GOOGLE_NAMING_CONVERSION_GUIDE.md) - Detailed conversion rules guide

### Important Notes

**Name improvement during conversion**: During the naming convention conversion, **you are not required to keep the original names**. You may freely improve function, variable, and constant names to make the code clearer and more understandable, as long as the new naming convention is followed.

**Name improvement examples**:

- `GetData()` → `getData()` (case change only) or `getConfigData()` (improved for clarity)
- `m_itemID` → `itemId_` (format change only) or `scheduleItemId_` (improved for clarity)
- `DEF_GLBDATA_CATE_NONE` → `kDefGlbdataCateNone` (format change only) or `kGlobalDataCategoryNone` (improved for clarity)

**Principles**:

- Prioritize clarity and readability
- Avoid unclear abbreviations
- Names must accurately describe purpose and meaning
- Ensure consistency across the entire project

### Execution Steps

#### Phase 1.1: Functions

- [ ] Create automated script/tool if possible
- [ ] Convert one file at a time, starting with core files
- [ ] Convert function names from `PascalCase` to `camelCase`
- [ ] Update all function calls
- [ ] Test after each file to ensure no regressions
- [ ] Commit in small groups of files

#### Phase 1.2: Member Variables

- [ ] Convert member variables from `m_camelCase` to `camelCase_`
- [ ] Update all references to member variables
- [ ] Test and verify

#### Phase 1.3: Local Variables and Parameters

- [ ] Review local variables and parameters (usually already camelCase)
- [ ] Fix only cases that deviate from the standard
- [ ] Test and verify

#### Phase 1.4: Constants

- [ ] Convert preprocessor defines to constexpr where possible
- [ ] Convert static constexpr in classes
- [ ] Test and verify

#### Phase 1.5: Namespaces

- [ ] Convert namespace names from `PascalCase` to `snake_case`
- [ ] Update all references to namespaces
- [ ] Test and verify

#### Phase 1.6: Global Variables

- [ ] Convert global variables from `g_` + Hungarian to `g_` + `camelCase_`
- [ ] Update all references
- [ ] Test and verify

#### Phase 1.7: Structs and Enums

- [ ] Convert struct names if needed
- [ ] Convert enum values if needed
- [ ] Test and verify

### Estimated Timeline

- Phase 1.1: 2-3 weeks (Functions)
- Phase 1.2: 2-3 weeks (Member Variables)
- Phase 1.3: 1 week (Local Variables and Parameters)
- Phase 1.4: 1 week (Constants)
- Phase 1.5: 1 week (Namespaces)
- Phase 1.6: 1 week (Global Variables)
- Phase 1.7: 1 week (Structs and Enums)

**Total for Phase 1**: ~9-12 weeks

---

## Phase 2: Code Structure and Organization

### Objective

Improve code structure, separate concerns, and reorganize modules.

### Planned Scope

- Clearly separate interface and implementation
- Reorganize utility classes
- Improve dependency management
- Optimize include guards and forward declarations

### Estimated Timeline

~4-6 weeks

---

## Phase 3: Modern C++ Features

### Objective

Apply modern C++ features (C++11/14/17/20) to improve code quality.

### Planned Scope

- Use smart pointers instead of raw pointers
- Use constexpr and consteval
- Use std::optional, std::variant where appropriate
- Improve exception handling
- Use range-based for loops
- Use auto where appropriate

### Estimated Timeline

~6-8 weeks

---

## Phase 4: Testing and Documentation

### Objective

Add unit tests and improve documentation.

### Planned Scope

- Add unit tests for core modules
- Improve code comments and documentation
- Create API documentation

### Estimated Timeline

~4-6 weeks

---

## Phase 5: Performance and Optimization

### Objective

Optimize performance and memory usage.

### Planned Scope

- Profile and identify bottlenecks
- Optimize critical paths
- Improve memory management
- Reduce unnecessary copies

### Estimated Timeline

~3-4 weeks

---

## Execution Strategy

### 1. Incremental Approach

- Refactor one file at a time
- Thoroughly test after each change
- Commit frequently with clear messages

### 2. Version Control

- Create separate branches for each phase
- Merge into main branch after completion and thorough testing
- Tag releases after each completed phase

### 3. Testing

- Ensure all tests pass before committing
- Manual testing for key features
- Regression testing after each phase

### 4. Code Review

- Code review for every pull request
- Ensure compliance with new naming conventions
- Ensure no breaking changes

### 5. Documentation

- Update documentation in parallel with code changes
- Keep conversion guide up-to-date
- Document decisions and trade-offs

### 6. Name Improvement During Conversion

- **Not required to keep original names**: Feel free to improve names for clarity
- **Prioritize clarity**: New names should be clearer than the old ones
- **Avoid abbreviations**: Remove unclear abbreviations
- **Consistency**: Ensure new names are consistent with the rest of the codebase
- **Document changes**: Note significant name changes in commit messages

---

## Risks and Mitigation

### Risks

1. **Breaking changes**: May introduce bugs if not careful
2. **Time consuming**: Refactoring takes significant time
3. **Merge conflicts**: May occur if multiple people work simultaneously

### Mitigation

1. **Thorough testing**: Test extensively after each change
2. **Small commits**: Commit small and frequently
3. **Communication**: Clear team communication about changes
4. **Backup**: Ensure backups and rollback capability

---

## Completion Criteria

### Phase 1 is considered complete when

- [ ] All functions use `camelCase`
- [ ] All member variables use `camelCase_`
- [ ] All local variables and parameters use `camelCase`
- [ ] All constants use `kConstantName`
- [ ] All namespaces use `snake_case`
- [ ] All global variables use `g_` + `camelCase_`
- [ ] Code compiles successfully
- [ ] All tests pass
- [ ] Manual testing shows no regressions
- [ ] Code review approved

---

## Related Documents

- [GOOGLE_NAMING_CONVERSION_GUIDE.md](./GOOGLE_NAMING_CONVERSION_GUIDE.md) - Detailed conversion rules
- [NAMING_CONVENTIONS.md](./NAMING_CONVENTIONS.md) - Current naming conventions

---

**This document will be updated as the plan changes.**
**Created**: 2025-01-XX
**Version**: 1.0
