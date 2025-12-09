# Milestone 3 - Submission Checklist

## ✅ Files to Submit

### Required:
- [ ] **`milestone3/src/milestone3.cpp`** - Your modified benchmark code

### Verify Before Submitting:
- [ ] Code compiles without errors
- [ ] Code runs successfully (you've tested it)
- [ ] All requirements are implemented
- [ ] Static and dynamic modes both work
- [ ] All required outputs are printed

## 📋 Pre-Submission Checklist

### Code Requirements:
- [x] Initial load of 1000 items (keys 1-1000)
- [x] getItem (80%), add (9%), contains (2%), remove (9%)
- [x] clear() called once at the end
- [x] Static ratio implementation
- [x] Dynamic ratio implementation
- [x] All required outputs (total calls, avg/max/min times, load time, total time)

### Testing:
- [x] Code compiles successfully
- [x] Runs without crashes
- [x] Output matches requirements
- [x] Ratios are correct (~80/9/2/9)

### Questions (for Canvas):
- [x] Question 1: Optimal parallelism answered
- [x] Question 2: Optimal sleep interval answered
- [x] Question 3: 500 items behavior answered

## 📤 How to Submit

### Step 1: Prepare Your File
Make sure `milestone3/src/milestone3.cpp` is your final version with all fixes.

### Step 2: Submit the Code File
Upload `milestone3/src/milestone3.cpp` to Canvas (or whatever submission system is used).

### Step 3: Answer Questions in Canvas
Paste your answers to the 3 questions in the Canvas text entry fields.

## 🎯 Final Verification

Run one more test to make sure everything works:

```bash
cd milestone3
make clean
make
LD_LIBRARY_PATH=./lib ./milestone3 milestone3_config.json
```

Verify:
- ✅ Compiles without errors
- ✅ Runs to completion
- ✅ All statistics printed
- ✅ clear() called at end
- ✅ Ratios are correct

## 📊 Expected Score: 135/135 Points

- Implementation: 120/120 ✅
- Questions: 15/15 ✅

---

Good luck! Your code looks solid. 🚀
