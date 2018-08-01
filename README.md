## reprocessing-ui
Simple UI elements for reprocessing because we all need simple UI elements for editor UI and all sorts of other stuff.

Example project using this: [https://bsansouci.github.io/fft.re](https://bsansouci.github.io/fft.re)

### checkbox
```reason
let makeCheckbox = (~checked=false : bool, env : Reprocessing.glEnvT) => UI.checkboxT;

let drawCheckbox = (
  ~checkbox : UI.checkboxT, 
  ~text="" : string, 
  ~pos : (int, int), 
  env : Reprocessing.glEnvT
) => UI.checkboxT;
```

Simply make one and put it in your state, then call `drawCheckbox`. The checkbox draw its own text to its side because it handles clicks on the text automatically.

```reason
type stateT = {
  myCheckbox: UI.checkboxT,
};

let setup = env => {
  { myCheckbox: UI.makeCheckbox(env) };
};

let draw = (state, env) => {
  Draw.background(Utils.color(~r=199, ~g=217, ~b=229, ~a=255), env);
  
  let myCheckbox = UI.drawCheckbox(~checkbox=state.myCheckbox, ~text="Click Me!", ~pos=(100, 100), env);

  {...state, myCheckbox}
};

run(~setup, ~draw, ());
```

The `drawCheckbox` function returns the new state of the checkbox, make sure to add it to your state at the end of render.

**properties**:

- `checked` : bool — Whether the checkbox is checked or unchecked. Will toggle as soon as the click is registered.
- `animationState` : checkboxTransitionT — The state if the checkbox animation, useful to sync the checkbox being ticked/unticked with some other UI
- `time` : float — The current animation time. Grows between 0 and `animationTime`
- `animationTime` : float — The total animation time. Useful to sync other pieces of UI with the checkbox animating

### slider
```reason
let makeSlider = (
  ~defaultValue=50. : float,
  ~min=0. : float,
  ~max=100. : float,
  _env : Reprocessing.glEnvT,
) => UI.sliderT;

let drawSlider = (
  ~slider : UI.sliderT, 
  ~pos : (int, int), 
  env : Reprocessing.glEnvT,
) => UI.sliderT;
```

Simply make one and put it in your state, then call `drawSlider`.

```reason
type stateT = {
  mySlider: UI.sliderT,
};

let setup = env => {
  { mySlider: UI.makeSlider(env) };
};

let draw = (state, env) => {
  Draw.background(Utils.color(~r=199, ~g=217, ~b=229, ~a=255), env);
  
  let mySlider = UI.drawSlider(~slider=state.mySlider, ~pos=(100, 100), env);

  {...state, mySlider}
};

run(~setup, ~draw, ());
```

The `drawSlider` function returns the new state of the slider, make sure to add it to your state at the end of render.

**properties**:

- `value` : float — The value of the slider, between min and max (min included).
- `min` : float — The min value that the slider can take (included)
- `max` : float — The max value that the slider can take (excluded)
- `selected` : bool — Whether the user is pressing down on the knob of the slider
