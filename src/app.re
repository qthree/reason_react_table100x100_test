[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

/* State declaration */
type state = {
  data: array(array(int))
};

/* Action declaration */
type action =
  | Click(int, int)
  | Initialize
  ;

let component = ReasonReact.reducerComponent("App");

let hypotenuse = (x, y) => {
  sqrt(x*.x +. y*.y)
};

let distance_of_difference = (x_dif, y_dif, max_dist) => {
  let distance = hypotenuse(float_of_int(x_dif), float_of_int(y_dif));
  max_dist - min(truncate(4.0*.distance), max_dist)
};

let colorify = (number: int) => {
  Printf.sprintf("rgb(%d, %d, 150)", number, number)
};

let make = (~message, _children) => {
  ...component,
  initialState: () => {data: [||]},
  reducer: (action, state) =>
    switch (action) {
    | Click(x, y) => {
      let data = Array.mapi((row_index, row) => {
        Array.mapi((column_index, _item) => {
          distance_of_difference(x-column_index, y-row_index, 255)
        }, row)
      }, state.data);
      ReasonReact.Update({data: data})
    }
    | Initialize => {
        let data = Array.make(100, Array.make(100, 0));
        ReasonReact.Update({data: data})
    }
  },
  render: self =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement(message)) </h2>
        <button onClick=(_event => self.send(Initialize))>(ReasonReact.stringToElement("Initialize"))</button>
      </div>
      <div className="App-intro">
        <table><tbody>
            (
            ReasonReact.arrayToElement(
              Array.mapi
              ( (row_index, row) => 
                <tr key=(string_of_int(row_index))> (ReasonReact.arrayToElement(
                  Array.mapi
                  ( (column_index, item) => <td
                    key=(Printf.sprintf("%d_%d", row_index, column_index))
                    onClick=(_event => self.send(Click(column_index, row_index)))
                    style={
                      ReactDOMRe.Style.make(~backgroundColor=colorify(item), ())
                    }
                  >
                  </td>
                  , row
                  )
                )) </tr>
              , self.state.data
            )
            ))
        </tbody></table>
      </div>
    </div>
};
