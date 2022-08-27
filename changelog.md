## ATtiny_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ATtiny_TimerInterrupt.svg?)](https://www.ardu-badge.com/ATtiny_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ATtiny_TimerInterrupt.svg)](https://github.com/khoih-prog/ATtiny_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ATtiny_TimerInterrupt.svg)](http://github.com/khoih-prog/ATtiny_TimerInterrupt/issues)

## Table of Contents

* [Changelog](#changelog)
  * [Release v1.0.1](#release-v101)
  * [Initial Release v1.0.0](#initial-release-v100)

---
---

## Changelog

### Release v1.0.1

1. Fix bug giving error when using TCB0 (`USE_TIMER_0 == true`)
2. Default to use TCB0 for boards having only 1 TCB Timer, such as `ATtiny817`, `ATtiny807`

### Initial Release v1.0.0

1. Intial release to support Arduino **AVR ATtiny-based boards (ATtiny3217, etc.) using megaTinyCore**
2. Add notes `howto upload by drag-and-drop` to `CURIOSITY` virtual drive


