## reprocessing-ui
Simple UI elements for reprocessing because we all need simple UI elements for editor UI and all sorts of other stuff.

### checkbox
Simply make one and put it in your state, then call `drawCheckbox` with the checkbox and the position.

```reason
type stateT = {
  myCheckbox: UI.checkboxT,
};

let setup = env => {
  { myCheckbox: UI.makeCheckbox(env) };
};

let draw = (state, env) => {
  Draw.background(Utils.color(~r=199, ~g=217, ~b=229, ~a=255), env);
  
  let myCheckbox = UI.drawCheckbox(~checkbox=state.myCheckbox, ~pos=(100, 100), env);
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
